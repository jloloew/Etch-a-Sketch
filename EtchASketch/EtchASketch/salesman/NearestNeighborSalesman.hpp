//
//  NearestNeighborSalesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 2/3/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef NearestNeighborSalesman_hpp
#define NearestNeighborSalesman_hpp

#include <unordered_set>
#include <vector>
#include "KDPoint.hpp"
#include "KDTree.hpp"
#include "Salesman.hpp"

namespace etchasketch {
namespace salesman {

/// Solves the traveling salesman problem.
class NearestNeighborSalesman : public Salesman {

  public:
	/// The startPoint must be contained within the unorderedPoints.
	NearestNeighborSalesman(const std::unordered_set<KDPoint<2>> &unorderedPoints,
	                        const KDPoint<2> &startPoint);

	virtual ~NearestNeighborSalesman();

	/// Order the points for the best drawing order.
	virtual void orderPoints();

	/// Get a copy of the ordered points.
	const std::vector<KDPoint<2>> *getOrderedPoints() const
	{
		return new std::vector<KDPoint<2>>(orderedPoints);
	}

  protected:
	const KDPoint<2> startPoint;
	std::unordered_set<KDPoint<2>> unorderedPoints;
	std::vector<KDPoint<2>> orderedPoints;

  private:
	/**
	 * Add the nearest neighbor to the last point added until we run out
	 * of points to add.
	 *
	 * @discussion This algorithm produces lots of large lines all the
	 * way across the image.
	 */
	void nearestNeighborAlgorithm(KDTree<2> &kdTree);
};
}
}

#endif /* NearestNeighborSalesman_hpp */
