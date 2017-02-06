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

using namespace boost;

#pragma mark - Implementation

namespace etchasketch {
namespace salesman {

SmallishSpanningTreeWalkSalesman::SmallishSpanningTreeWalkSalesman(
        const std::unordered_set<KDPoint<2>> &unorderedPoints, const KDPoint<2> &startPoint)
    : Salesman(),
      startPoint(startPoint),
      unorderedPoints(unorderedPoints),
      orderedPoints(vector<KDPoint<2>>())
{
}

void SmallishSpanningTreeWalkSalesman::orderPoints()
{
	// Create a K-D tree with the points.
	KDTree<2> *kdTree = new KDTree<2>(unorderedPoints);

	smallishSpanningTreeWalkAlgorithm(*kdTree);

	delete kdTree;
}

void SmallishSpanningTreeWalkSalesman::smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree)
{
	// Map each point to its vertex_t.
	std::unordered_map<KDPoint<2>, VertexDesc> *vertexMap =
	        new std::unordered_map<KDPoint<2>, VertexDesc>();

	// Initialize a graph with all the points as vertices, but no edges.
	UndirectedGraph *g = new UndirectedGraph(unorderedPoints.size());
	for (auto iter = unorderedPoints.begin(); iter != unorderedPoints.end(); ++iter) {
		// Add the point to the graph as a vertex.
		const KDPoint<2> &pt = *iter;
		VertexDesc v = add_vertex(*g);
		(*g)[v] = pt;
		(*vertexMap)[pt] = v;
	}

	const VertexDesc startVertex = (*vertexMap)[startPoint];

	// 1. For each point, add an edge from it to its nearest neighbor.
	for (auto iter = unorderedPoints.begin(); iter != unorderedPoints.end(); ++iter) {
		const KDPoint<2> &pt = *iter;
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
	}
	delete vertexMap;
	vertexMap = nullptr;

	// 2. For each disjoint subgraph, find its average coordinate (its center).
	// Find the disjoint subgraphs.
	VertexDescMap *idxMap = new VertexDescMap();
	associative_property_map<VertexDescMap> indexMap(*idxMap);
	VertexIterator di, dj;
	tie(di, dj) = vertices(*g);
	for (size_t i = 0; di != dj; ++di, ++i) {
		put(indexMap, *di, i);
	}

	VertexDescMap *compMap = new VertexDescMap();
	associative_property_map<VertexDescMap> componentMap(*compMap);

	size_t numComponents =
	        connected_components(*g, componentMap, vertex_index_map(indexMap));
	if (numComponents > 1) {
		// Turn each component into a set of vertices.
		vector<std::unordered_set<VertexDesc>> components(numComponents);
		for (size_t i = 0; i != numComponents; ++i) {
			components[i] = std::unordered_set<VertexDesc>();
		}
		// Add each vertex to the set representing its component.
		for (auto iter = compMap->begin(); iter != compMap->end(); ++iter) {
			size_t componentIndex = iter->second;
			VertexDesc vDesc = iter->first;
			std::unordered_set<VertexDesc> &component = components[componentIndex];
			component.insert(vDesc);
		}

		// Connect the components.
		connectComponents(*g, components);
	}
	// Clean up.
	delete idxMap;
	idxMap = nullptr;
	delete compMap;
	compMap = nullptr;

	// The graph is now connected. We can walk the graph with a DFS to get the
	// order of the points for drawing.
	walkDFS(*g, startVertex);

	// A little bit of clean up, and we're done!
	delete g;
}

void SmallishSpanningTreeWalkSalesman::connectComponents(UndirectedGraph &g,
                                                         vector<GraphComponent> &components) const
{
	// Find the center of each component.

	/// Keys are the points at the center of the components (the values).
	std::unordered_map<const KDPoint<2>, GraphComponent &> componentCenters;
	std::unordered_map<const GraphComponent *, KDTree<2>> compTrees;
	KDTree<2> compCentersTree; // A KDTree with the center of each component.

	bool isFirstComponent = true;
	KDPoint<2> centerA; // The center of component A.
	for (auto iter = components.begin(); iter != components.end(); ++iter) {
		GraphComponent &comp = *iter;
		// Set up a KDTree of the points in the component.
		compTrees[&comp] = KDTree<2>();
		KDTree<2> &compTree = compTrees[&comp];
		// Find the average coords of all the points in the component.
		const KDPoint<2> avgPoint = findCenterPoint(g, comp, compTree);
		componentCenters.insert({ { avgPoint, comp } });

		// Check whether this is the first component. Don't insert the first one
		// into the KDTree.
		if (isFirstComponent) {
			isFirstComponent = false;
			centerA = avgPoint;
			continue;
		}
		// Add the point to the KDTree.
		if (!compCentersTree.insert(avgPoint)) {
			EASLog("Error: center point already in use by another component. "
			       "Skipping.");
		}
	}

	// Loop until we have one single giant component.
	while (components.size() >= 2) {
		// Debug
		if (components.size() % 100 == 0) {
			EASLog("%lu components remaining", components.size());
		}

		// Pick an arbitrary component (the first one). Find the component
		// whose center is nearest to this one.
		GraphComponent &compA = components[0];
		KDPoint<2> *nn = compCentersTree.findNearestNeighbor(centerA);
		const KDPoint<2> compCenterB = *nn;
		delete nn;
		nn = nullptr;
		GraphComponent &compB = componentCenters.at(compCenterB);

		// debug
		if (compB.size() <= 0) {
			EASLog("compB.size = %lu", compB.size());
			assert(compB.size() > 0);
		}

		// Find the point in component B nearest the center of component A. This
		// point will become B's bridging point with A.
		VertexDesc bridgeB = findNearestPoint(g, compB, centerA);
		// Find the point in A nearest the bridge point in B.
		const KDPoint<2> bridgeBPt = g[bridgeB];
		VertexDesc bridgeA = findNearestPoint(g, compA, bridgeBPt);

		// Create an edge between bridge points A and B.
		add_edge(bridgeA, bridgeB, g);

		// Merge component A into component B.
		mergeComponents(compB, compA, compCenterB, centerA, g, componentCenters,
		                compTrees, compCentersTree);
		// Remove A from the component set and delete it.
		components.erase(components.begin());
	}

	// debug
	if (true) {
		size_t sz = components[0].size();
		EASLog("Size of last component: %lu", sz);
	}
}

KDPoint<2> SmallishSpanningTreeWalkSalesman::findCenterPoint(const UndirectedGraph &g,
                                                             const GraphComponent &comp,
                                                             KDTree<2> &compTree) const
{
	// Find the average of all the points in the component.
	KDPoint<2> avgPoint(0, 0);
	for (auto vDesc = comp.begin(); vDesc != comp.end(); ++vDesc) {
		const KDPoint<2> pt = g[*vDesc];
		// Sum each point.
		avgPoint[0] += pt[0];
		avgPoint[1] += pt[1];
		// Add each point we find to the component's KDTree.
		compTree.insert(pt);
	}
	// Divide each component by the number of points.
	for (int i = 0; i <= 1; ++i) {
		double sum = static_cast<double>(avgPoint[i]);
		sum /= static_cast<double>(comp.size());
		avgPoint[i] = static_cast<KDPointCoordinate>(sum);
	}
	return avgPoint;
}

SmallishSpanningTreeWalkSalesman::VertexDesc
        SmallishSpanningTreeWalkSalesman::findNearestPoint(const UndirectedGraph &g,
                                                           const GraphComponent &component,
                                                           const KDPoint<2> &target) const
{
	// Find the point within the component nearest the target point.
	float bestDist = std::numeric_limits<float>::max();
	const VertexDesc *bestDesc = nullptr;
	for (auto iter = component.begin(); iter != component.end(); ++iter) {
		const VertexDesc &vDesc = *iter;
		const KDPoint<2> pt = g[vDesc];
		const float curDist = pt.distanceTo(target);
		if (curDist < bestDist) {
			bestDist = curDist;
			bestDesc = &vDesc;
		}
	}
	return *bestDesc;
}

void SmallishSpanningTreeWalkSalesman::mergeComponents(
        GraphComponent &dst,
        GraphComponent &src,
        const KDPoint<2> &centerDst,
        const KDPoint<2> &centerSrc,
        const UndirectedGraph &g,
        std::unordered_map<const KDPoint<2>, GraphComponent &> &componentCenters,
        std::unordered_map<const GraphComponent *, KDTree<2>> &compTrees,
        KDTree<2> &compCentersTree) const
{
	// Update the center of the soon-to-be-merged component.
	KDPoint<2> newCenterDst(0, 0);
	for (int i = 0; i <= 1; ++i) {
		// Recompute the average coordinate in each dimension.
		newCenterDst[i] += centerDst[i] * dst.size();
		newCenterDst[i] += centerSrc[i] * src.size();
		double sum = static_cast<double>(newCenterDst[i]);
		sum /= static_cast<double>(dst.size() + src.size());
		newCenterDst[i] = static_cast<KDPointCoordinate>(sum);
	}
	// Merge the points of src into dst.
	dst.insert(src.begin(), src.end());
	// Erase the center of src and the old center of dst from the
	// componentCenters map, and add the center of the new, merged component.
	componentCenters.erase(centerSrc);
	componentCenters.erase(centerDst);
	assert(dst.size() > 0);
	componentCenters.insert({ { newCenterDst, dst } });
	// Do the same with the compCentersTree.
	compCentersTree.remove(centerSrc);
	compCentersTree.remove(centerDst);
	compCentersTree.insert(newCenterDst);
	// Add the points of src to the KDTree of dst.
	KDTree<2> &dstTree = compTrees[&dst];
	for (auto iter = src.begin(); iter != src.end(); ++iter) {
		const KDPoint<2> pt = g[*iter];
		dstTree.insert(pt);
	}
	// Delete src's KDTree.
	compTrees.erase(&src);
}

void SmallishSpanningTreeWalkSalesman::walkDFS(const UndirectedGraph &g,
                                               const VertexDesc &startVertex)
{
	// Mark all vertices as unvisited to start.
	std::unordered_map<VertexDesc, bool> visited =
	        std::unordered_map<VertexDesc, bool>();
	VertexIterator vi, vEnd;
	tie(vi, vEnd) = vertices(g);
	for (; vi != vEnd; ++vi) {
		visited[*vi] = false;
	}
	// Call the helper.
	walkDFSHelper(g, startVertex, visited);
}

// Returns true if curVertex was successfully visited for the first time, or
// false otherwise.
bool SmallishSpanningTreeWalkSalesman::walkDFSHelper(const UndirectedGraph &g,
                                                     const VertexDesc &curVertex,
                                                     std::unordered_map<VertexDesc, bool> &visited)
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

} // namespace salesman
} // namespace etchasketch
