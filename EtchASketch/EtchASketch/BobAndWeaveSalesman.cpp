//
//  BobAndWeaveSalesman.cpp
//  EtchASketch
//
//  Created by Justin Loew on 2/3/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "BobAndWeaveSalesman.hpp"
#include "EASUtils.hpp"
#include <cmath>


namespace etchasketch {
namespace salesman {

BobAndWeaveSalesman::BobAndWeaveSalesman(const Image &grayscaleImage,
										 const Image &edgeImage)
    : Salesman(), grayscaleImage(grayscaleImage), edgeImage(edgeImage)
{
}

BobAndWeaveSalesman::~BobAndWeaveSalesman()
{
}

void BobAndWeaveSalesman::orderPoints()
{
	// TODO: parallelize this
	for (KDPointCoordinate y = 0; y < grayscaleImage.getHeight(); y += lineSeparation) {
		// First go left to right…
		for (KDPointCoordinate x = 0; x < grayscaleImage.getWidth(); ++x) {
			orderedPoints.push_back(offsetPointAt(x, y));
		}

		// Then go right to left.
		y += lineSeparation;
		if (y >= grayscaleImage.getHeight()) {
			break;
		}

		// Then go right to left.
		for (KDPointCoordinate x = static_cast<int>(grayscaleImage.getWidth()) - 1;
		     x >= 0; --x) {
			orderedPoints.push_back(offsetPointAt(x, y));
		}
	}
}

KDPoint<2> BobAndWeaveSalesman::offsetPointAt(const KDPointCoordinate x,
                                              const KDPointCoordinate y) const
{
	uint32_t totalBrightness = 0;
	uint32_t numPixelsSearched = 0;
	// Search within the square radius.
	for (KDPointCoordinate xi = MAX(x - pointSearchRadius, 0);
	     xi < MIN(x + pointSearchRadius, grayscaleImage.getWidth()); ++xi) {
		for (KDPointCoordinate yi = MAX(y - pointSearchRadius, 0);
		     yi < MIN(y + pointSearchRadius, grayscaleImage.getHeight()); ++yi) {
			// Check if this pixel is part of an edge.
			const KDPoint<2> pt(xi, yi);
			const Image::Pixel px = grayscaleImage[pt];
			uint8_t brightness = (px >> 8) & 0xFF;
			totalBrightness += brightness;
			
			++numPixelsSearched;
		}
	}
	double avgBrightness = static_cast<double>(totalBrightness) / numPixelsSearched / 255.0;
	double verticalDisplacement = sin(x) * avgBrightness * lineSeparation / 2 * k_grav;
	
	// Add the vertical displacement to the point and return it.
	KDPointCoordinate dy = static_cast<KDPointCoordinate>(verticalDisplacement);
	return KDPoint<2>(x, y + dy);
}

} // namespace salesman
} // namespace etchasketch
