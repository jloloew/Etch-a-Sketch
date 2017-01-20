//
//  SmallishSpanningTreeWalkSalesman.cpp
//  EtchASketch
//
//  Created by Justin Loew on 12/11/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "SmallishSpanningTreeWalkSalesman.hpp"
#include "EASUtils.hpp"
#include <algorithm>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using std::vector;
using etchasketch::KDTree;
using etchasketch::KDPoint;
using etchasketch::salesman::Salesman;

using namespace boost;


#pragma mark - Implementation


etchasketch::salesman::SmallishSpanningTreeWalkSalesman::SmallishSpanningTreeWalkSalesman(const std::unordered_set<KDPoint<2>> &unorderedPoints,
																						  const KDPoint<2> &startPoint)
: Salesman(unorderedPoints, startPoint)
{ }

void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::orderPoints()
{
	// Create a K-D tree with the points.
	KDTree<2> *kdTree = new KDTree<2>(unorderedPoints);
	
	smallishSpanningTreeWalkAlgorithm(*kdTree);
	
	delete kdTree;
}

/*
 * 1. Add an edge from each node to its nearest neighbor.
 * 2. For each disjoint subgraph, find its average coordinate (its center).
 * 3. Add the center of each subgraph to a KDTree.
 * 4. Recurse (go to step 1).
 * 5. For each edge between subgraphs, use a heuristic to connect them. Let the
 *    subgraphs be called A and B. Create a KDTree with the vertices in B. Find
 *    the nearest neighbor to the center of subgraph A. This will be the
 *    endpoint in B used to bridge to A. Add the vertices in A to a KDTree. Find
 *    the nearest neighbor to the bridge vertex we found in B. Connect the
 *    vertices in A and B.
 */
void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree)
{
	
	// Map each point to its vertex_t.
	std::map<KDPoint<2>, VertexDesc> *vertexMap = new std::map<KDPoint<2>, VertexDesc>();
	
	// Initialize a graph with all the points as vertices, but no edges.
	UndirectedGraph *g = new UndirectedGraph(unorderedPoints.size());
	std::for_each(unorderedPoints.begin(), unorderedPoints.end(), [&](const KDPoint<2> &pt) {
		// Add the point to the graph as a vertex.
		VertexDesc v = add_vertex(*g);
		(*g)[v] = pt;
		(*vertexMap)[pt] = v;
	});
	
	const VertexDesc startVertex = (*vertexMap)[startPoint];
	
	// 1. For each point, add an edge from it to its nearest neighbor.
	std::for_each(unorderedPoints.begin(), unorderedPoints.end(), [&](const KDPoint<2> &pt) {
		// Find the point's nearest neighbor.
		const KDPoint<2> *nn = kdTree.findNearestNeighbor(pt);
		if ((nullptr != nn) && nn->isValid()) {
			// Add an edge from the point to its nearest neighbor.
			EdgeDesc e;
			bool inserted __unused;
			tie(e, inserted) = add_edge((*vertexMap)[pt], (*vertexMap)[*nn], *g);
		}
		// Clean up.
		delete nn;
	});
	delete vertexMap;
	
	// 2. For each disjoint subgraph, find its average coordinate (its center).
	// Find the disjoint subgraphs.
	// TODO: Build this information as we go with incremental_components().
	
	VertexDescMap *idxMap = new VertexDescMap();
	associative_property_map<VertexDescMap> indexMap(*idxMap);
	VertexIterator di, dj;
	tie(di, dj) = vertices(*g);
	for (size_t i = 0; di != dj; ++di, ++i) {
		put(indexMap, *di, i);
	}
	
	VertexDescMap *compMap = new VertexDescMap();
	associative_property_map<VertexDescMap> componentMap(*compMap);
	
	size_t numComponents = connected_components(*g, componentMap, vertex_index_map(indexMap));
	if (numComponents > 1) {
		// Turn each component into a set of vertices.
		vector<std::unordered_set<VertexDesc> *> components(numComponents);
		for (size_t i = 0; i != numComponents; ++i) {
			components[i] = new std::unordered_set<VertexDesc>();
		}
		// Add each vertex to the set representing its component.
		for (auto iter = compMap->begin(); iter != compMap->end(); ++iter) {
			size_t componentIndex = iter->second;
			VertexDesc vDesc = iter->first;
			std::unordered_set<VertexDesc> *component = components[componentIndex];
			component->insert(vDesc);
		}
		
		// Connect the components.
		connectComponents(*g, components);
		
		// Free the components.
		std::for_each(components.begin(), components.end(), [](std::unordered_set<VertexDesc> *component) {
			delete component;
		});
	}
	// Clean up.
	delete idxMap;
	delete compMap;
	
	// The graph is now connected. We can walk the graph with a DFS to get the
	// order of the points for drawing.
	walkDFS(*g, startVertex);
	
	// A little bit of clean up, and we're done!
	delete g;
}

void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::connectComponents(UndirectedGraph &g,
																		   vector<GraphComponent *> &components)
{
	
	// Find the center of each component.
	std::unordered_map<const GraphComponent *, KDPoint<2>> *componentCenters = new std::unordered_map<const GraphComponent *, KDPoint<2>>();
	std::for_each(components.begin(), components.end(), [&](const GraphComponent *comp) {
		// Find the average coords of all the points in the component.
		KDPoint<2> avgPoint(0, 0);
		std::for_each(comp->begin(), comp->end(), [&g, &avgPoint](const VertexDesc vDesc) {
			const KDPoint<2> pt = g[vDesc];
			avgPoint[0] += pt[0];
			avgPoint[1] += pt[1];
		});
		avgPoint[0] /= comp->size();
		avgPoint[1] /= comp->size();
		(*componentCenters)[comp] = avgPoint;
	});
	
	// Loop until we have one single giant component.
	while (components.size() >= 2) {
		// Debug
		if (components.size() % 100 == 0) {
			EASLog("%lu components remaining", components.size());
		}
		
		// Pick an arbitrary component (the first one). Find the component nearest
		// to this one.
		GraphComponent *compA = components[0];
		GraphComponent *compB = components[1];
		const KDPoint<2> centerA = (*componentCenters)[compA];
		float bestDist = centerA.distanceTo((*componentCenters)[compB]);
		for (int i = 2; i != components.size(); ++i) {
			GraphComponent *curComp = components[i];
			const float curDist = centerA.distanceTo((*componentCenters)[curComp]);
			if (curDist < bestDist) {
				bestDist = curDist;
				compB = curComp;
			}
		}
		
		// Find the point in component B nearest the center of component A. This
		// point will become B's bridging point with A.
		VertexDesc bridgeB = findNearestPoint(g, *compB, centerA);
		// Find the point in A nearest the bridge point in B.
		const KDPoint<2> centerB = g[bridgeB];
		VertexDesc bridgeA = findNearestPoint(g, *compA, centerB);
		
		// Create an edge between bridges A and B.
		add_edge(bridgeA, bridgeB, g);
		
		// Update the center of the soon-to-be-merged component.
		// (We're going to merge the points of A into B.)
		KDPoint<2> newCenterB(0, 0);
		newCenterB[0] += centerB[0] * compB->size();
		newCenterB[0] += centerA[0] * compA->size();
		newCenterB[0] /= compA->size() + compB->size();
		newCenterB[1] += centerB[1] * compB->size();
		newCenterB[1] += centerA[1] * compA->size();
		newCenterB[1] /= compA->size() + compB->size();
		(*componentCenters)[compB] = newCenterB;
		// For performance reasons, we don't even bother to erase compA from
		// componentCenters.
		
		// Merge the points of A into B.
		compB->insert(compA->begin(), compA->end());
		components.erase(components.begin());
		delete compA;
	}
	
	// Clean up.
	delete componentCenters;
	componentCenters = nullptr;
}

etchasketch::salesman::SmallishSpanningTreeWalkSalesman::VertexDesc
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::findNearestPoint(const UndirectedGraph &g,
																		  const GraphComponent &component,
																		  const KDPoint<2> &target) const
{
	// Find the point within the component nearest the target point.
	float bestDist = std::numeric_limits<float>::max();
	const VertexDesc *bestDesc = nullptr;
	std::for_each(component.begin(), component.end(), [&](const VertexDesc vDesc) {
		const KDPoint<2> pt = g[vDesc];
		const float curDist = pt.distanceTo(target);
		if (curDist < bestDist) {
			bestDist = curDist;
			bestDesc = &vDesc;
		}
	});
	return *bestDesc;
}

void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::walkDFS(const UndirectedGraph &g,
																 const VertexDesc &startVertex)
{
	// Mark all vertices as unvisited to start.
	std::map<VertexDesc, bool> *visited = new std::map<VertexDesc, bool>();
	VertexIterator vi, vEnd;
	tie(vi, vEnd) = vertices(g);
	for (; vi != vEnd; ++vi) {
		(*visited)[*vi] = false;
	}
	// Call the helper.
	walkDFSHelper(g, startVertex, *visited);
	// Clean up and return.
	delete visited;
}

// Returns true if curVertex was successfully visited for the first time, or
// false otherwise.
bool
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::walkDFSHelper(const UndirectedGraph &g,
																	   const VertexDesc &curVertex,
																	   std::map<VertexDesc, bool> &visited)
{
	// Base case.
	if (visited[curVertex]) {
		return false;
	}
	
	// Mark the current vertex as visited.
	visited[curVertex] = true;
	// Add this vertex's point to the list of ordered points.
	const KDPoint<2> curPoint = g[curVertex];
	orderedPoints.push_back(curPoint);
	// Check each adjacent vertex.
	AdjacencyIterator ai, aEnd;
	tie(ai, aEnd) = adjacent_vertices(curVertex, g);
	for (; ai != aEnd; ++ai) {
		// Try to visit each adjacent vertex.
		VertexDesc v = *ai;
		bool didVisit = walkDFSHelper(g, v, visited);
		if (didVisit) {
			// Step back up to the current point after each adjacent vertex is
			// explored.
			orderedPoints.push_back(curPoint);
		}
	}
	// This vertex was explored successfully.
	return true;
}
















