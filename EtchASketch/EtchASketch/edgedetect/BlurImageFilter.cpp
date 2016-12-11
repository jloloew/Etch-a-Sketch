//
//  BlurImageFilter.cpp
//  EtchASketch
//
//  Created by Justin Loew on 12/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "BlurImageFilter.hpp"
#include <math.h>

using etchasketch::Image;
using Pixel = etchasketch::Image::Pixel;

etchasketch::edgedetect::BlurImageFilter::BlurImageFilter()
: etchasketch::edgedetect::ImageFilter()
{
	// Precompute our kernel.
	initKernel();
}

void
etchasketch::edgedetect::BlurImageFilter::initKernel()
{
	// Based on http://stackoverflow.com/questions/8204645/implementing-gaussian-blur-how-to-calculate-convolution-matrix-kernel
	float sigma = 1.0f;
	float mean = radius / 2;
	float sum = 0.0f; // For accumulating the kernel values
	for (uint32_t x = 0; x < radius; x++)
		for (uint32_t y = 0; y < radius; y++) {
			kernel[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
			/ (2 * M_PI * sigma * sigma);
			
			// Accumulate the kernel values
			sum += kernel[x][y];
		}
	
	// Normalize the kernel
	for (uint32_t x = 0; x < radius; x++) {
		for (uint32_t y = 0; y < radius; y++) {
			kernel[x][y] /= sum;
		}
	}
}

Image *
etchasketch::edgedetect::BlurImageFilter::apply(const Image &originalImage) const
{
	// Create the image into which we'll write our blurred image.
	Image *blurredImage = new Image(originalImage.getWidth() - 2*radius,
									originalImage.getHeight() - 2*radius);
	
	// Blur each pixel.
	for (int x = radius; x < originalImage.getWidth() - radius; x++) {
		for (int y = radius; y < originalImage.getHeight() - radius; y++) {
			const KDPoint<2> destPt(x - radius, y - radius);
			(*blurredImage)[destPt] = blurredPixel(originalImage, x, y);
		}
	}
	
	return blurredImage;
}

Pixel
etchasketch::edgedetect::BlurImageFilter::blurredPixel(const Image &img,
													   int x, int y) const
{
	// Perform convolution to combine the region of the image with the kernel.
	float sum = 0.0f;
	int dx;
	for (dx = -radius; dx <= (int)radius; dx++) {
		for (int dy = -radius; dy <= (int)radius; dy++) {
			const KDPoint<2> pt(x + dx, y + dy);
			const float kernelVal = kernel[dx+radius][dy+radius];
			const Pixel pix = (img[pt] >> 8) & 0xFF;
			const float pixVal = static_cast<float>(pix);
			sum += pixVal * kernelVal;
		}
	}
	// Turn it back into a Pixel.
	uint8_t channel = static_cast<uint8_t>(sum);
	Pixel retVal = (channel << 24) |
				   (channel << 16) |
				   (channel <<  8) |
				   0xFF;
	return retVal;
}
