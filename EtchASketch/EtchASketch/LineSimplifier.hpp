//
//  LineSimplifier.hpp
//  EtchASketch
//
//  Created by Justin Loew on 4/4/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef LineSimplifier_hpp
#define LineSimplifier_hpp

#include <list>
#include <vector>
#include "KDPoint.hpp"

namespace etchasketch {

/**
 * Removes extra points to "simplify" a line while maintaining a certain level
 * of quality.
 */
class LineSimplifier {
public:
	/**
	 * Create a new @c LineSimplifier.
	 * 
	 * @param epsilon The minimum distance a point needs to be from a line in
	 * order to be kept.
	 */
	LineSimplifier(float epsilon = 50.0f);
	
	/**
	 * Remove any points from the line if its removal doesn't change the path
	 * taken by too much.
	 * 
	 * @note This method is not reentrant.
	 *
	 * @param line (inout) The ordered points that make up the line.
	 */
	void simplifyLine(std::vector<etchasketch::KDPoint<2>> &line);
	
private:
	/// The minimum distance a point must be from a line in order to be kept.
	const float epsilon;
	
	/// A doubly linked list containing the line we're currently working on.
	std::list<etchasketch::KDPoint<2>> *line;
	
	/**
	 * Implementation of @c simplifyLine(). Uses the Douglas-Peucker line
	 * simplification algorithm.
	 *
	 * @param start The first point in the line segment.
	 * @param end The last point in the line segment.
	 */
	void
	douglasPeucker(const std::list<etchasketch::KDPoint<2>>::iterator &start,
				   const std::list<etchasketch::KDPoint<2>>::iterator &end)
	const;
};

}

#endif /* LineSimplifier_hpp */
