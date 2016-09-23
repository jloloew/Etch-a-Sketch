//
//  Image.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "Image.hpp"

etchasketch::Image::Image(size_t width, size_t height)
:Image(nullptr, width, height)
{ }

etchasketch::Image::Image(const Pixel *data, size_t width, size_t height)
:width(width), height(height)
{
	// TODO: check for overflow on the multiplication
	this->data = new Image::Pixel[width * height];
	if (nullptr != data) {
		memcpy(this->data, data, width * height * sizeof(etchasketch::Image::Pixel));
	}
}

etchasketch::Image::~Image()
{
	delete [] data;
	data = nullptr;
}

size_t
etchasketch::Image::getWidth(void) const
{
	return width;
}

size_t
etchasketch::Image::getHeight(void) const
{
	return height;
}

const etchasketch::Image::Pixel &
etchasketch::Image::operator[](const Point<2> &index) const
{
	size_t dataIndex = index[0] + (index[1] * width);
	return data[dataIndex];
}

etchasketch::Image::Pixel &
etchasketch::Image::operator[](const Point<2> &index)
{
	size_t dataIndex = index[0] + (index[1] * width);
	return data[dataIndex];
}
