//
//  ImageFlow.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "ImageFlow.hpp"
#include "SobelEdgeDetector.hpp"
#include "BlurImageFilter.hpp"
#include "NearestNeighborSalesman.hpp"
#include "LineSimplifier.hpp"

using std::unordered_set;
using std::vector;
using etchasketch::Image;
using etchasketch::KDPoint;
using etchasketch::edgedetect::BlurImageFilter;
using etchasketch::edgedetect::SobelEdgeDetector;
using etchasketch::salesman::Salesman;
using etchasketch::salesman::NearestNeighborSalesman;

etchasketch::ImageFlow::ImageFlow(const Image &colorImage)
: originalImage(colorImage),
grayscaleImage(colorImage.getWidth(), colorImage.getHeight()),
edgeDetectedImage(Image(0, 0)),
edgePoints(nullptr),
orderedEdgePoints(nullptr),
scaledEdgePoints(nullptr),
edgeDetector(new SobelEdgeDetector()),
salesman(nullptr),
outputWidth(colorImage.getWidth()),
outputHeight(colorImage.getHeight())
{ }

etchasketch::ImageFlow::~ImageFlow()
{
	delete edgePoints;
	delete orderedEdgePoints;
	delete scaledEdgePoints;
	delete edgeDetector;
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
			grayscaleImage[pt] = 0xFF | (gray << 8) | (gray << 16) | (gray << 24);
		}
	}
}

void
etchasketch::ImageFlow::detectEdges()
{
	/*
	// Blur the image.
	BlurImageFilter blurFilter = BlurImageFilter();
	Image *blurredImage = blurFilter.apply(grayscaleImage);
	// Perform edge detection.
	Image *detectedImage = edgeDetector->detectEdges(*blurredImage);
	delete blurredImage;
	blurredImage = nullptr;
	/*/
	Image *detectedImage = edgeDetector->detectEdges(grayscaleImage);
	// */
	edgeDetectedImage = *detectedImage;
}

void
etchasketch::ImageFlow::generateEdgePoints()
{
	unordered_set<KDPoint<2>> *pointSet = new unordered_set<KDPoint<2>>();
	// Loop through each point to see if its pixel is part of an edge.
	for (int x = 0; x < edgeDetectedImage.getWidth(); x++) {
		for (int y = 0; y < edgeDetectedImage.getHeight(); y++) {
			const KDPoint<2> pt(x, y);
			const Image::Pixel px = edgeDetectedImage[pt];
			// Arbitrarily choose the green component. RGB all have the same value.
			const uint8_t greenComponent = ((px >> 16) & 0xFF);
			// Check for non-black.
			if (greenComponent != 0) {
				pointSet->insert(pt);
			}
		}
	}
	
	// Insert the starting point if it's not already in there.
	const KDPoint<2> startPoint(0, 0);
	pointSet->insert(startPoint);
	
	setEdgePoints(pointSet);
}

void
etchasketch::ImageFlow::orderEdgePointsForDrawing()
{
	// TODO: Put startPoint in class scope or something.
	const KDPoint<2> startPoint(0, 0);
	Salesman *salesman = nullptr;
	salesman = new NearestNeighborSalesman(*edgePoints, startPoint);
	setSalesman(salesman);
	salesman->orderPoints();
	vector<KDPoint<2>> *line = new vector<KDPoint<2>>(salesman->getOrderedPoints());
	setSalesman(nullptr); // Done with the salesman.
	
	// Simplify the line.
	LineSimplifier lineSimplifier = LineSimplifier();
	lineSimplifier.simplifyLine(*line);
	
	setOrderedEdgePoints(line);
}

void
etchasketch::ImageFlow::scalePointsToFitOutputSize()
{
	vector<KDPoint<2>> *scaledPoints = new vector<KDPoint<2>>();
	scaledPoints->reserve(orderedEdgePoints->size());
	
	for (auto it = orderedEdgePoints->begin(); it != orderedEdgePoints->end(); ++it) {
		const KDPoint<2> &ipt = *it;
		KDPointCoordinate mptx, mpty;
		mptx = static_cast<KDPointCoordinate>(floor(ipt[0] * outputWidth / static_cast<float>(edgeDetectedImage.getWidth())));
		mpty = static_cast<KDPointCoordinate>(floor(ipt[1] * outputHeight / static_cast<float>(edgeDetectedImage.getHeight())));
		scaledPoints->push_back(KDPoint<2>(mptx, mpty));
	}
	
	setScaledEdgePoints(scaledPoints);
}

const vector<KDPoint<2>> &
etchasketch::ImageFlow::getFinalPoints()
{
	// Make sure we actually have the ordered edge points ready to go.
	performAllComputationSteps();
	return *scaledEdgePoints;
}

void
etchasketch::ImageFlow::performAllComputationSteps()
{
	// Check if each stage of computation is done. If any stage has not yet been
	// performed, do so now.
	if (!edgePoints) {
		convertToGrayscale();
		detectEdges();
		generateEdgePoints();
	}
	
	if (!orderedEdgePoints) {
		orderEdgePointsForDrawing();
	}
	
	if (!scaledEdgePoints) {
		scalePointsToFitOutputSize();
	}
}

void
etchasketch::ImageFlow::setOutputSize(size_t width, size_t height)
{
	outputWidth = width;
	outputHeight = height;
	// Remove our current scaled edge points.
	setScaledEdgePoints(nullptr);
}

#pragma mark Setters

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
etchasketch::ImageFlow::setScaledEdgePoints(const std::vector<etchasketch::KDPoint<2> > *newScaledEdgePoints)
{
	// Delete the old value and set it to the new pointer.
	delete scaledEdgePoints;
	scaledEdgePoints = newScaledEdgePoints;
}

void
etchasketch::ImageFlow::setSalesman(Salesman *newSalesman)
{
	// Delete the old value and set it to the new pointer.
	delete salesman;
	salesman = newSalesman;
}
