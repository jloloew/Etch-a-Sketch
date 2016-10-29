//
//  SobelEdgeDetector.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "SobelEdgeDetector.hpp"

using etchasketch::Image;
using Pixel = etchasketch::Image::Pixel;

etchasketch::edgedetect::SobelEdgeDetector::SobelEdgeDetector()
: etchasketch::edgedetect::EdgeDetector()
{ }

Image *
etchasketch::edgedetect::SobelEdgeDetector::detectEdges(
		const Image &grayscaleImage) const
{
	// dst's width and height are each 1 less than the grayscaleImage.
	Image *dst = new Image(grayscaleImage.getWidth()-1,
						   grayscaleImage.getHeight()-1);
	
	for (int x = 1; x < grayscaleImage.getWidth() - 1; x++) {
		for (int y = 1; y < grayscaleImage.getHeight() - 1; y++) {
			KDPoint<2> dstPt(x-1, y-1);
			float intensity = intensityForPoint(grayscaleImage, x, y);
			// Scale the float intensity into an RGBA pixel.
			uint8_t intensityInt = static_cast<uint8_t>(intensity / 255.0f);
			Pixel intensityPixel =    (0xFF         << 24)
									| (intensityInt << 16)
									| (intensityInt <<  8)
									|  intensityInt;
			(*dst)[dstPt] = intensityPixel;
		}
	}
	
	return dst;
}

float
etchasketch::edgedetect::SobelEdgeDetector::intensityForPoint(const Image &img,
															  const int x,
															  const int y) const
{
	static float sobelX[3][3] = {
		{ -1.0f, -2.0f, -1.0f },
		{  0.0f,  0.0f,  0.0f },
		{  1.0f,  2.0f,  1.0f }
	};
	static float sobelY[3][3] = {
		{ 1.0f, 0.0f, -1.0f },
		{ 2.0f, 0.0f, -2.0f },
		{ 1.0f, 0.0f, -1.0f }
	};
	
	// Find the gradient across this pixel.
	float sumX = 0.0f;
	float sumY = 0.0f;
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			KDPoint<2> pt(x + dx, y + dy);
			float currentIntensity = static_cast<float>(img[pt]);
			sumX += sobelX[dx+1][dy+1] * currentIntensity;
			sumY += sobelY[dx+1][dy+1] * currentIntensity;
		}
	}
	
	float outputVal; // 1.0 to output white, 0.0 to output black.
	
	// Filter out values below a threshold.
	float thresholdX = 0.6f;
	float thresholdY = 0.4f;
	if (sumX >= thresholdX || sumY >= thresholdY) {
		outputVal = 1.0f;
	} else {
		outputVal = 0.0f;
	}
	
	return outputVal;
}
