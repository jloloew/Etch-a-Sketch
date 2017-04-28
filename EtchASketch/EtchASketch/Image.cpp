//
//  Image.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "Image.hpp"
#include <cmath>

using std::out_of_range;
using Pixel = etchasketch::Image::Pixel;
using etchasketch::KDPoint;

static
void
scale(const Pixel oldData[], size_t oldWidth, size_t oldHeight,
	  Pixel newData[], size_t newWidth, size_t newHeight);

static
void
downsample(const Pixel oldData[], size_t oldWidth, size_t oldHeight,
		   Pixel newData[], size_t newWidth, size_t newHeight);

static inline
void
doOldY(size_t y, float yf, float oldYf, float nextOldYf,
	   size_t x, float oldPixelsPerNewPixelX,
	   size_t oldWidth, const Pixel oldData[],
	   float newPixSum[4]); // newPixSum is an output (array of size 4).

static inline
void
doOldX(float oldXf, float nextOldXf, size_t oldY,
	   size_t oldWidth, const Pixel oldData[],
	   float newPixSum[4]); // newPixSum is an output (array of size 4).

etchasketch::Image::Image(size_t width, size_t height, const Pixel *data)
: width(width), height(height)
{
	this->data = new Pixel[getPixelCount()];
	if (nullptr == this->data) {
		this->width = 0;
		this->height = 0;
		this->data = nullptr;
		return;
	}
	if (nullptr != data) {
		memcpy(this->data, data, getPixelCount() * sizeof(Pixel));
	} else {
		memset(this->data, 0, getPixelCount() * sizeof(Pixel));
	}
	return;
}

etchasketch::Image::Image(const etchasketch::Image &other,
						  size_t newWidth,
						  size_t newHeight)
: width(newWidth), height(newHeight)
{
	data = new Pixel[getPixelCount()];
	// TODO: Handle upscaling.
	downsample(other.getData(), other.getWidth(), other.getHeight(),
			   data, width, height);
}

etchasketch::Image::Image(const etchasketch::Image &other)
: width(other.getWidth()), height(other.getHeight())
{
	data = new Pixel[other.getPixelCount()];
	if (nullptr != data) {
		memcpy(data, other.data, getPixelCount() * sizeof(Pixel));
	}
}

etchasketch::Image &
etchasketch::Image::operator=(const etchasketch::Image &that)
{
	if (this == &that) { // Safety first
		return *this;
	}
	
	width = that.getWidth();
	height = that.getHeight();
	delete [] data;
	const size_t pxCount = that.getPixelCount();
	data = new Pixel[pxCount];
	memcpy(data, that.getData(), pxCount * sizeof(Pixel));
	return *this;
}

etchasketch::Image::~Image()
{
	delete [] data;
	data = nullptr;
}

void
etchasketch::Image::resize(size_t newWidth, size_t newHeight)
{
	// Scale the old image into the new size using a separate buffer.
	Pixel *newData = new Pixel[newWidth * newHeight];
	// TODO: Handle upscaling.
	scale(data, width, height,
		  newData, newWidth, newHeight);
	delete [] data;
	data = newData;
	width = newWidth;
	height = newHeight;
}

static
void
scale(const Pixel oldData[], size_t oldWidth, size_t oldHeight,
	  Pixel newData[], size_t newWidth, size_t newHeight)
{
	if (newWidth <= oldWidth && newHeight <= oldHeight) {
		downsample(oldData, oldWidth, oldHeight,
				   newData, newWidth, newHeight);
	} else if (newWidth >= oldWidth && newHeight >= oldHeight) {
		// TODO: Handle upsampling.
	} else {
		// TODO: Figure out what to do for changing the aspect ratio.
	}
}

#define INDEX_FOR_COORDS(x, y, rowWidth) (((y) * (rowWidth)) + (x))

static
void
downsample(const Pixel oldData[], size_t oldWidth, size_t oldHeight,
		   Pixel newData[], size_t newWidth, size_t newHeight)
{
	float owf = static_cast<float>(oldWidth);
	float ohf = static_cast<float>(oldHeight);
	float nwf = static_cast<float>(newWidth);
	float nhf = static_cast<float>(newHeight);
	float oldPixelsPerNewPixelX = owf / nwf; // This breaks if the aspect ratio
	float oldPixelsPerNewPixelY = ohf / nhf; // is changing.
	for (size_t y = 0; y < newHeight; y++) {
		float yf = static_cast<float>(y);
		float oldYf = yf * oldPixelsPerNewPixelY;
		float nextOldYf = (yf + 1) * oldPixelsPerNewPixelY;
		for (size_t x = 0; x < newWidth; x++) {
			// Average the components of each pixel within this area of the
			// original image.
			float newPixSum[4] = { 0.0f };
			doOldY(y, yf, oldYf, nextOldYf, x, oldPixelsPerNewPixelX, oldWidth,
				   oldData, newPixSum);
			Pixel newPix = 0;
			for (int i = 0; i < 4; i++) {
				uint8_t component = static_cast<uint8_t>(newPixSum[i]);
				newPix |= component << (8*i);
			}
			newData[INDEX_FOR_COORDS(x, y, newWidth)] = newPix;
		}
	}
}

static
void
doOldY(size_t y, float yf, float oldYf, float nextOldYf,
	   size_t x, float oldPixelsPerNewPixelX,
	   size_t oldWidth, const Pixel oldData[],
	   float newPixSum[4])
{
	float xf = static_cast<float>(x);
	float oldXf = xf * oldPixelsPerNewPixelX;
	float nextOldXf = (xf + 1) * oldPixelsPerNewPixelX;
	float numRowsScanned = 0.0f;
	// Add the first (fractional) pixel.
	size_t oldY = static_cast<size_t>(floorf(oldYf));
	if (oldY != ceilf(oldYf)) {
		doOldX(oldXf, nextOldXf, oldY, oldWidth, oldData, newPixSum);
		float fracOfPixToBeUsed = (ceilf(oldYf) - oldYf);
		for (int i = 0; i < 4; i++) {
			// Add each component.
			newPixSum[i] += fracOfPixToBeUsed * newPixSum[i];
		}
		numRowsScanned += fracOfPixToBeUsed;
		oldY++;
	}
	// Add all the middle pixels.
	for (; oldY < static_cast<size_t>(floorf(nextOldYf)); oldY++) {
		doOldX(oldXf, nextOldXf, oldY, oldWidth, oldData, newPixSum);
		numRowsScanned += 1;
	}
	// Add the last (fractional) pixel.
	if (oldY < nextOldYf) {
		float tmpNewPixSum[4] = { 0.0f };
		doOldX(oldXf, nextOldXf, oldY, oldWidth, oldData, tmpNewPixSum);
		float fracOfPixToBeUsed = (nextOldYf - floorf(nextOldYf));
		for (int i = 0; i < 4; i++) {
			// Add each component.
			newPixSum[i] += fracOfPixToBeUsed * tmpNewPixSum[i];
		}
		numRowsScanned += fracOfPixToBeUsed;
	}
	// Divide down the sum into the average and add it to the
	// accumulator.
	for (int i = 0; i < 4; i++) {
		float component = newPixSum[i] / numRowsScanned;
		newPixSum[i] += component;
	}
}

static
void
doOldX(float oldXf, float nextOldXf, size_t oldY,
	   size_t oldWidth, const Pixel oldData[],
	   float newPixSum[4]) // pixSum is an output (array of size 4).
{
	float newPixSumX[4] = { 0.0f }; // RGBA
	float numPixelsScanned = 0.0f;
	// Add the first (fractional) pixel.
	size_t oldX = static_cast<size_t>(floorf(oldXf));
	if (oldX != ceilf(oldXf)) {
		Pixel pix = oldData[INDEX_FOR_COORDS(oldX, oldY, oldWidth)];
		float fracOfPixToBeUsed = (ceilf(oldXf) - oldXf);
		for (int i = 0; i < 4; i++) {
			// Add each component.
			float component = static_cast<float>((pix >> (8*i)) & 0xFF);
			newPixSumX[i] += fracOfPixToBeUsed * component;
		}
		numPixelsScanned += fracOfPixToBeUsed;
		oldX++;
	}
	// Add all the middle pixels.
	for (; oldX < static_cast<size_t>(floorf(nextOldXf)); oldX++) {
		Pixel pix = oldData[INDEX_FOR_COORDS(oldX, oldY, oldWidth)];
		for (int i = 0; i < 4; i++) {
			// Add each component.
			float component = static_cast<float>((pix >> (8*i)) & 0xFF);
			newPixSumX[i] += component;
		}
		numPixelsScanned += 1;
	}
	// Add the last (fractional) pixel.
	if (oldX < nextOldXf) {
		Pixel pix = oldData[INDEX_FOR_COORDS(oldX, oldY, oldWidth)];
		float fracOfPixToBeUsed = (nextOldXf - floorf(nextOldXf));
		for (int i = 0; i < 4; i++) {
			// Add each component.
			float component = static_cast<float>((pix >> (8*i)) & 0xFF);
			newPixSumX[i] += fracOfPixToBeUsed * component;
		}
		numPixelsScanned += fracOfPixToBeUsed;
	}
	// Divide down the sum into the average and add it to the
	// accumulator.
	for (int i = 0; i < 4; i++) {
		float component = newPixSumX[i] / numPixelsScanned;
		newPixSum[i] += component;
	}
}

#define VALIDATE_INDEX(index) do { \
	if ((index)[0] < 0 || (index)[0] >= getWidth()) { \
		out_of_range e("Invalid X coordinate"); \
		throw e; \
	} \
	if ((index)[1] < 0 || (index)[1] >= getHeight()) { \
		out_of_range e("Invalid Y coordinate"); \
		throw e; \
	} \
} while (0)

Pixel
etchasketch::Image::operator[](const KDPoint<2> &index) const
{
	VALIDATE_INDEX(index);
	size_t dataIndex = index[0] + (index[1] * width);
	return data[dataIndex];
}

Pixel &
etchasketch::Image::operator[](const KDPoint<2> &index)
{
	VALIDATE_INDEX(index);
	size_t dataIndex = index[0] + (index[1] * width);
	return data[dataIndex];
}
