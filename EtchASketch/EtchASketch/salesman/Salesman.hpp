//
//  Salesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef Salesman_hpp
#define Salesman_hpp

#include <unordered_set>
#include <vector>
#include "KDTree.hpp"

namespace etchasketch {
	namespace salesman {
		
		/// Solves the traveling salesman problem.
		class Salesman {
			
		public:
			Salesman(const std::unordered_set<etchasketch::KDPoint<2>>
					 &unorderedPoints);
			
			virtual ~Salesman();
			
			/// Order the points for the best drawing order.
			void orderPoints();
			
			/// Get a copy of the ordered points.
			const std::vector<KDPoint<2>> * getOrderedPoints() const
				{ return new std::vector<KDPoint<2>>(orderedPoints); }
			
		private:
			std::unordered_set<KDPoint<2>> unorderedPoints;
			std::vector<KDPoint<2>> orderedPoints;
			
			void nearestNeighborAlgorithm(KDTree<2> &kdTree);
			void primsAlgorithm(KDTree<2> &kdTree);
		};
		
	}
}

#endif /* Salesman_hpp */
