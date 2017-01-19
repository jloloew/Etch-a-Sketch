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

namespace etchasketch {
	namespace salesman {
		
		class SmallishSpanningTreeWalkSalesman : public Salesman {
		public:
			SmallishSpanningTreeWalkSalesman(const std::unordered_set<etchasketch::KDPoint<2>> &unorderedPoints);
			
			virtual ~SmallishSpanningTreeWalkSalesman() { };
			
			/// Order the points for the best drawing order.
			virtual void orderPoints();
			
		private:
			/**
			 *
			 */
			void smallishSpanningTreeWalkAlgorithm(KDTree<2> &kdTree);
		};
		
	}
}

#endif /* SmallishSpanningTreeWalkSalesman_hpp */
