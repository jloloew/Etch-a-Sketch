//
//  BobAndWeaveSalesman.cpp
//  EtchASketch
//
//  Created by Justin Loew on 2/3/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "BobAndWeaveSalesman.hpp"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SIGN(a) ((a) < 0 ? -1 : 1)

namespace etchasketch {
namespace salesman {

BobAndWeaveSalesman::BobAndWeaveSalesman(const Image &edgeImage, size_t lineSeparation)
    : Salesman(), edgeImage(edgeImage), lineSeparation(lineSeparation)
{
}

BobAndWeaveSalesman::~BobAndWeaveSalesman()
{
}

void BobAndWeaveSalesman::orderPoints()
{
	// TODO: parallelize this
	for (KDPointCoordinate y = 0; y < edgeImage.getHeight(); y += lineSeparation) {
		// First go left to right…
		for (KDPointCoordinate x = 0; x < edgeImage.getWidth(); ++x) {
			orderedPoints.push_back(offsetPointAt(x, y));
		}

		// Then go right to left.
		y += lineSeparation;
		if (y >= edgeImage.getHeight()) {
			break;
		}

		// Then go right to left.
		for (KDPointCoordinate x = static_cast<int>(edgeImage.getWidth()) - 1;
		     x >= 0; --x) {
			orderedPoints.push_back(offsetPointAt(x, y));
		}
	}
}

KDPoint<2> BobAndWeaveSalesman::offsetPointAt(const KDPointCoordinate x,
                                              const KDPointCoordinate y) const
{
	float verticalDisplacement = 0.0f;
	const KDPoint<2> centerPoint(x, y);
	// Search within the square radius.
	for (KDPointCoordinate xi = MAX(x - pointSearchRadius, 0);
	     xi < MIN(x + pointSearchRadius, edgeImage.getWidth()); ++xi) {
		for (KDPointCoordinate yi = MAX(y - pointSearchRadius, 0);
		     yi < MIN(y + pointSearchRadius, edgeImage.getHeight()); ++yi) {
			if (xi == x && yi == y) {
				// Avoid dividing by zero when we measure the distance from the
				// center to the center.
				continue;
			}
			// Check if this pixel is part of an edge.
			const KDPoint<2> pt(xi, yi);
			const Image::Pixel px = edgeImage[pt];
			uint8_t greenComponent = (px >> 8) & 0xFF;
			if (greenComponent == 0) {
				// This is part of an edge.
				verticalDisplacement += SIGN(yi - y) / pt.distanceTo(centerPoint);
			}
		}
	}
	// Add the vertical displacement to the point and return it.
	KDPointCoordinate dy = static_cast<KDPointCoordinate>(verticalDisplacement);
	return KDPoint<2>(centerPoint[0], centerPoint[1] + dy);
}

} // namespace salesman
} // namespace etchasketch
