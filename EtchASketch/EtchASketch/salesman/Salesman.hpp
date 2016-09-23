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
#include "Image.hpp"
#include "KDTree.hpp"

using etchasketch::Image;

namespace etchasketch {
	namespace salesman {
		
		/// Solves the traveling salesman problem.
		class Salesman {
			
		public:
			Salesman(const std::unordered_set<Point<2> *> &unorderedPoints);
			
			virtual ~Salesman(void);
			
			/// Order the points for the best drawing order.
			void orderPoints(void);
			
			const std::vector<Point<2>> & getOrderedPoints(void) const
				{ return orderedPoints; }
			
		private:
			std::unordered_set<Point<2> *> unorderedPoints;
			std::vector<Point<2>> orderedPoints;
			
			void primsAlgorithm(KDTree<2> &kdTree);
		};
		
	}
}

#endif /* Salesman_hpp */
