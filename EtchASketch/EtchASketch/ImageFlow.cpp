//
//  ImageFlow.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "ImageFlow.hpp"

using std::unordered_set;
using std::vector;
using etchasketch::Image;
using etchasketch::edgedetect::EdgeDetector;
using etchasketch::salesman::Salesman;

etchasketch::ImageFlow::ImageFlow(const Image &colorImage)
: originalImage(colorImage),
grayscaleImage(colorImage.getWidth(), colorImage.getHeight()),
edgeDetectedImage(nullptr),
edgePoints(nullptr),
orderedEdgePoints(nullptr),
edgeDetector(),
salesman(nullptr)
{ }

etchasketch::ImageFlow::~ImageFlow()
{
	delete edgeDetectedImage;
	delete edgePoints;
	delete orderedEdgePoints;
	delete salesman;
}

void
etchasketch::ImageFlow::convertToGrayscale()
{
	// Transform each pixel.
	for (int x = 0; x < originalImage.getWidth(); x++) {
		for (int y = 0; y < originalImage.getHeight(); y++) {
			const KDPoint<2> pt(x, y);
			// Average the components.
			const Image::Pixel color = originalImage[pt];
			const Image::Pixel gray =   (((color >> 24) & 0xFF)
									   + ((color >> 16) & 0xFF)
									   + ((color >>  8) & 0xFF)) / 3;
			grayscaleImage[pt] = gray;
		}
	}
}

void
etchasketch::ImageFlow::detectEdges()
{
	setEdgeDetectedImage(edgeDetector.detectEdges(grayscaleImage));
}

void
etchasketch::ImageFlow::generateEdgePoints()
{
	unordered_set<KDPoint<2>> *pointSet = new unordered_set<KDPoint<2>>();
	// Loop through each point to see if its pixel is part of an edge.
	for (int x = 0; x < edgeDetectedImage->getWidth(); x++) {
		for (int y = 0; y < edgeDetectedImage->getHeight(); y++) {
			const KDPoint<2> pt(x, y);
			const Image::Pixel px = (*edgeDetectedImage)[pt];
			const uint8_t greenComponent = ((px >> 16) & 0xFF);
			// Check for non-black.
			if (greenComponent != 0) {
				pointSet->insert(pt);
			}
		}
	}
	
	setEdgePoints(pointSet);
}

void
etchasketch::ImageFlow::orderEdgePointsForDrawing()
{
	setSalesman(new Salesman(*edgePoints));
	salesman->orderPoints();
	setOrderedEdgePoints(&salesman->getOrderedPoints());
	// Done with the salesman.
	setSalesman(nullptr);
}

#pragma mark Setters

void
etchasketch::ImageFlow::setEdgeDetectedImage(const Image *newImage)
{
	// Delete the old value and set it to the new pointer.
	delete edgeDetectedImage;
	edgeDetectedImage = newImage;
}

void
etchasketch::ImageFlow::setEdgePoints(const unordered_set<KDPoint<2>>
									  *newEdgePoints)
{
	// Delete the old value and set it to the new pointer.
	delete edgePoints;
	edgePoints = newEdgePoints;
}

void
etchasketch::ImageFlow::setOrderedEdgePoints(const vector<KDPoint<2>>
											 *newOrderedEdgePoints)
{
	// Delete the old value and set it to the new pointer.
	delete orderedEdgePoints;
	orderedEdgePoints = newOrderedEdgePoints;
}

void
etchasketch::ImageFlow::setSalesman(Salesman *newSalesman)
{
	// Delete the old value and set it to the new pointer.
	delete salesman;
	salesman = newSalesman;
}
