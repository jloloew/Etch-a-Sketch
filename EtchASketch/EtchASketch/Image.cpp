//
//  Image.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "Image.hpp"

using std::out_of_range;
using Pixel = etchasketch::Image::Pixel;
using etchasketch::KDPoint;

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

etchasketch::Image::Image(const etchasketch::Image &other)
: width(other.getWidth()), height(other.getHeight())
{
	data = new Pixel[other.getPixelCount()];
	if (nullptr != data) {
		memcpy(data, other.data, getPixelCount() * sizeof(Pixel));
	}
}

etchasketch::Image::~Image()
{
	delete [] data;
	data = nullptr;
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
