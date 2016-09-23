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
using namespace etchasketch;
using etchasketch::Image;
using edgedetect::EdgeDetector;

etchasketch::ImageFlow::ImageFlow(const Image &colorImage)
: originalImage(Image(colorImage)),
grayscaleImage(Image(colorImage.getWidth(), colorImage.getHeight()))
{
	edgeDetector = etchasketch::edgedetect::EdgeDetector();
}

etchasketch::ImageFlow::~ImageFlow()
{
	delete edgeDetectedImage;
	
	// Delete the generated edge points if they haven't yet been given to the salesman.
	if ((edgePoints != nullptr) && (salesman == nullptr)) {
		for (auto iter = edgePoints->begin(); iter != edgePoints->end(); iter++) {
			delete *iter;
			// No need to remove it from edgePoints because we're about to
			// delete edgePoints.
		}
	}
	delete edgePoints;
	edgePoints = nullptr;
}

void
etchasketch::ImageFlow::convertToGrayscale(void)
{
	// Transform each pixel.
	for (size_t x = 0; x < originalImage.getWidth(); x++) {
		for (size_t y = 0; y < originalImage.getHeight(); y++) {
			Point<2> pt(x, y);
			// Average the components.
			const Image::Pixel color = originalImage[pt];
			const Image::Pixel gray =    (((color >> 24) & 0xFF)
									   + ((color >> 16) & 0xFF)
									   + ((color >>  8) & 0xFF)) / 3;
			grayscaleImage[pt] = gray;
		}
	}
}

void
etchasketch::ImageFlow::detectEdges(void)
{
	edgeDetectedImage = edgeDetector.detectEdges(grayscaleImage);
}

void
etchasketch::ImageFlow::generateEdgePoints(void)
{
	unordered_set<Point<2> *> *pointSet = new unordered_set<Point<2> *>();
	// Loop through each point to see if its pixel is part of an edge.
	for (size_t x = 0; x < edgeDetectedImage->getWidth(); x++) {
		for (size_t y = 0; y < edgeDetectedImage->getHeight(); y++) {
			Point<2> pt(x, y);
			Image::Pixel px = (*edgeDetectedImage)[pt];
			uint8_t greenComponent = ((px >> 16) & 0xFF);
			// Check for non-black.
			if (greenComponent != 0) {
				pointSet->insert(new Point<2>(pt));
			}
		}
	}
	
	edgePoints = pointSet;
}

void
etchasketch::ImageFlow::orderEdgePointsForDrawing(void)
{
	salesman = new salesman::Salesman(*edgePoints);
	salesman->orderPoints();
	orderedEdgePoints = &salesman->getOrderedPoints();
	// Done with the salesman.
	delete salesman;
	salesman = nullptr;
}
