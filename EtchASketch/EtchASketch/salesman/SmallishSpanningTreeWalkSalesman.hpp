//
//  SmallishSpanningTreeWalkSalesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 12/11/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef SmallishSpanningTreeWalkSalesman_hpp
#define SmallishSpanningTreeWalkSalesman_hpp

#include "Salesman.hpp"
#include <boost/graph/adjacency_list.hpp>
#include "KDPoint.hpp"
#include "KDTree.hpp"

namespace etchasketch {
namespace salesman {

class SmallishSpanningTreeWalkSalesman : public Salesman {
  public:
	SmallishSpanningTreeWalkSalesman(const std::unordered_set<etchasketch::KDPoint<2>> &unorderedPoints,
	                                 const KDPoint<2> &startPoint);

	virtual ~SmallishSpanningTreeWalkSalesman(){};

	/// Order the points for the best drawing order.
	virtual void orderPoints();

  private:
	// Create our undirected graph types.
	typedef boost::adjacency_list<boost::hash_setS, boost::hash_setS, boost::undirectedS, KDPoint<2>> UndirectedGraph;
	typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
	typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor VertexDesc;
	typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
	typedef boost::graph_traits<UndirectedGraph>::edge_descriptor EdgeDesc;
	typedef boost::graph_traits<UndirectedGraph>::adjacency_iterator AdjacencyIterator;
	typedef std::unordered_map<VertexDesc, size_t> VertexDescMap; // Based on
	// https://stackoverflow.com/questions/15432104/how-to-create-a-propertymap-for-a-boost-graph-using-lists-as-vertex-container
	typedef std::unordered_set<VertexDesc> GraphComponent;
	
	const KDPoint<2> startPoint;
	std::unordered_set<KDPoint<2>> unorderedPoints;
	std::vector<KDPoint<2>> orderedPoints;

	/*
	 * 1. Add an edge from each node to its nearest neighbor.
	 * 2. For each disjoint subgraph, find its average coordinate
	 * (its center).
	 * 3. Add the center of each subgraph to a KDTree.
	 * 4. Recurse (go to step 1).
	 * 5. For each edge between subgraphs, use a heuristic to
	 * connect them. Let the subgraphs be called A and B. Create a
	 * KDTree with the vertices in B. Find the nearest neighbor to
	 * the center of subgraph A. This will be the endpoint in B
	 * used to bridge to A. Add the vertices in A to a KDTree.
	 * Find the nearest neighbor to the bridge vertex we found in
	 * B. Connect the vertices in A and B.
	 */
	void smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree);

	/**
	 * Add edges between the disjoint components of the graph to
	 * create a single connected component.
	 */
	void connectComponents(UndirectedGraph &g, std::vector<GraphComponent> &components) const;

	/**
	 * Find the average coordinate of all the points in the
	 * component. Also fills the compTree with each point in the
	                   * component.
	 */
	KDPoint<2> findCenterPoint(const UndirectedGraph &g,
	                           const GraphComponent &comp,
	                           KDTree<2> &compTree) const;

	/**
	 * Within a component, find the point nearest a given target
	 * point.
	 */
	VertexDesc findNearestPoint(const UndirectedGraph &g,
	                            const GraphComponent &component,
	                            const KDPoint<2> &target) const;

	/**
	 * Merge together two components of a graph. Also updates a
	 * few other maps and things, for convenience.
	 */
	void mergeComponents(GraphComponent &dst,
	                     GraphComponent &src,
	                     const KDPoint<2> &centerDst,
	                     const KDPoint<2> &centerSrc,
	                     const UndirectedGraph &g,
	                     std::unordered_map<const KDPoint<2>, GraphComponent &> &componentCenters,
	                     std::unordered_map<const GraphComponent *, KDTree<2>> &compTrees,
	                     KDTree<2> &compCentersTree) const;

	/**
	 * Walk the graph, adding each point visited to the list of
	 * ordered points.
	 */
	void walkDFS(const UndirectedGraph &g, const VertexDesc &startVertex);

	/**
	 * Returns true if curVertex was successfully visited for the
	 * first time, or false otherwise.
	 */
	bool walkDFSHelper(const UndirectedGraph &g,
	                   const VertexDesc &curVertex,
	                   std::unordered_map<VertexDesc, bool> &visited);
};
}
}

#endif /* SmallishSpanningTreeWalkSalesman_hpp */
