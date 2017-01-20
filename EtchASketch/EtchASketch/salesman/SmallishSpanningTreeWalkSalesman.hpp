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

namespace etchasketch {
	namespace salesman {
		
		class SmallishSpanningTreeWalkSalesman : public Salesman {
		public:
			SmallishSpanningTreeWalkSalesman(const std::unordered_set<etchasketch::KDPoint<2>> &unorderedPoints,
											 const KDPoint<2> &startPoint);
			
			virtual ~SmallishSpanningTreeWalkSalesman() { };
			
			/// Order the points for the best drawing order.
			virtual void orderPoints();
			
		private:
			// Create our undirected graph types.
			typedef boost::adjacency_list<boost::setS, boost::setS, boost::undirectedS, KDPoint<2>> UndirectedGraph;
			typedef boost::graph_traits<UndirectedGraph>::vertex_iterator VertexIterator;
			typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor VertexDesc;
			typedef boost::graph_traits<UndirectedGraph>::edge_iterator EdgeIterator;
			typedef boost::graph_traits<UndirectedGraph>::edge_descriptor EdgeDesc;
			typedef boost::graph_traits<UndirectedGraph>::adjacency_iterator AdjacencyIterator;
			typedef std::map<VertexDesc, size_t> VertexDescMap; // Based on https://stackoverflow.com/questions/15432104/how-to-create-a-propertymap-for-a-boost-graph-using-lists-as-vertex-container
			
			/**
			 *
			 */
			void smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree);
			
			/**
			 *
			 */
			void connectComponents(UndirectedGraph &g, vector<std::unordered_set<VertexDesc> *> &components);
			
			/**
			 * Walk the graph, adding each point visited to the list of ordered 
			 * points.
			 */
			void
			walkDFS(const UndirectedGraph &g, const VertexDesc &startVertex);
			
			/**
			 * Returns true if curVertex was successfully visited for the first 
			 * time, or false otherwise.
			 */
			bool
			walkDFSHelper(const UndirectedGraph &g,
						  const VertexDesc &curVertex,
						  std::map<VertexDesc, bool> &visited);
		};
		
	}
}

#endif /* SmallishSpanningTreeWalkSalesman_hpp */
