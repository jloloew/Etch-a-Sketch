//
//  BobAndWeaveSalesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 2/3/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef BobAndWeaveSalesman_hpp
#define BobAndWeaveSalesman_hpp

#include "Salesman.hpp"
#include "Image.hpp"
#include "KDPoint.hpp"

namespace etchasketch {
namespace salesman {

/**
 * Create a salesman that draws periodic horizontal lines which are perturbed to
 * follow the edges in the input image.
 */
class BobAndWeaveSalesman : public Salesman {
  public:
	/**
	 * @param edgeImage The image produced by edge detection.
	 * @param lineSeparation The vertical distance between each horizontal line
	 * drawn, in pixels.
	 */
	BobAndWeaveSalesman(const Image &edgeImage);

	virtual ~BobAndWeaveSalesman();

	void orderPoints();

  private:
	const Image &edgeImage;

	/// The vertical distance between each horizontal line drawn, in pixels.
	constexpr static const size_t lineSeparation = 4;
	
	/// The radius around each point, within which other points can perform
	// "gravitational attraction" to draw the cursor closer.
	constexpr static const size_t pointSearchRadius = 10;
	
	constexpr static const float k_grav = 10.0f;
	
	/**
	 * Get the vertical displacement for the cursor at the given point within
	 * the image.
	 * 
	 * Search within a radius of @c pointSearchRadius around the given point,
	 * performing "gravitational attraction" to draw the cursor up or down to
	 * better conform to the detected lines.
	 */
	KDPoint<2> offsetPointAt(const KDPointCoordinate x,
							 const KDPointCoordinate y) const;
};

}
}

#endif /* BobAndWeaveSalesman_hpp */
