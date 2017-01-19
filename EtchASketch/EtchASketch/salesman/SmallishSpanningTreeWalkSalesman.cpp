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
#include <boost/graph/adjacency_list.hpp>

using std::unordered_set;
using std::vector;
using etchasketch::KDTree;
using etchasketch::KDPoint;
using etchasketch::salesman::Salesman;

using namespace boost;

etchasketch::salesman::SmallishSpanningTreeWalkSalesman::SmallishSpanningTreeWalkSalesman(const std::unordered_set<etchasketch::KDPoint<2>> &unorderedPoints)
: Salesman(unorderedPoints)
{
	
}

void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::orderPoints()
{
	// Create a K-D tree with the points.
	KDTree<2> *kdTree = new KDTree<2>(unorderedPoints);
	
	smallishSpanningTreeWalkAlgorithm(*kdTree);
	
	delete kdTree;
}

void
etchasketch::salesman::SmallishSpanningTreeWalkSalesman::smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree)
{
	// Create our undirected graph types.
	typedef KDPoint<2> Vertex;
	typedef adjacency_list<setS, setS, undirectedS, Vertex> UndirectedGraph;
	typedef graph_traits<UndirectedGraph>::vertex_descriptor vertex_t;
	typedef graph_traits<UndirectedGraph>::edge_descriptor edge_t;
	
	// Map each point to its vertex_t.
	std::map<KDPoint<2>, vertex_t> *vertexMap = new std::map<KDPoint<2>, vertex_t>();
	
	// Initialize a graph with all the points as vertices, but no edges.
	UndirectedGraph *g = new UndirectedGraph(unorderedPoints.size());
	std::for_each(unorderedPoints.begin(), unorderedPoints.end(), [&](const KDPoint<2> &pt) {
		// Add the point to the graph as a vertex.
		vertex_t v = add_vertex(*g);
		(*g)[v] = pt;
		(*vertexMap)[pt] = v;
	});
	
	// For each point, add an edge from it to its nearest neighbor.
	std::for_each(unorderedPoints.begin(), unorderedPoints.end(), [&](const KDPoint<2> &pt) {
		// Find the point's nearest neighbor.
		const KDPoint<2> *nn = kdTree.findNearestNeighbor(pt);
		if ((nullptr != nn) && nn->isValid()) {
			// Add an edge from the point to its nearest neighbor.
			edge_t e;
			bool inserted __unused;
			tie(e, inserted) = add_edge((*vertexMap)[pt], (*vertexMap)[*nn], *g);
			// TODO: Should I delete my copy of nn here? If not now, then when?
		}
		// Clean up.
		delete nn;
	});
	
	// Clean up allocated objects.
	delete vertexMap;
	delete g;
}
