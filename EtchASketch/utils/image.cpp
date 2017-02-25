/**
 * @file image.cpp
 * @author jloew2
 */

#include "image.h"
#include <iostream>

void Image::flipleft()
{
	// declare variables here to avoid repeat allocs
	RGBAPixel *p1, *p2;
	RGBAPixel *temp = new RGBAPixel;
	for(size_t yi = 0; yi < height(); yi++)
	{
		for(size_t xi = 0; xi < width() / 2; xi++)
		{
			p1 = operator()(xi, yi);
			p2 = operator()(width() - 1 - xi, yi);
			*temp = *p1;
			*p1 = *p2;
			*p2 = *temp;
		}
	}
	delete temp;
}

void Image::adjustbrightness(int r, int g, int b)
{
	for(size_t xi = 0; xi < width(); xi++)
	{
		for(size_t yi = 0; yi < height(); yi++)
		{
			RGBAPixel *pix = operator()(xi, yi);
			int red = pix->red + r;
			int green = pix->green + g;
			int blue = pix->blue + b;
			pix->red = red > 255 ? 255 : red < 0 ? 0 : red;
			pix->green = green > 255 ? 255 : green < 0 ? 0 : green;
			pix->blue = blue > 255 ? 255 : blue < 0 ? 0 : blue;
		}
	}
}

void Image::invertcolors()
{
	for(size_t xi = 0; xi < width(); xi++)
	{
		for(size_t yi = 0; yi < height(); yi++)
		{
			RGBAPixel *pix = operator()(xi, yi);
			pix->red = 255 - pix->red;
			pix->green = 255 - pix->green;
			pix->blue = 255 - pix->blue;
		}
	}
}

bool Image::etchToFile(string const & file_name) {
	FILE * fp = fopen(file_name.c_str(), "wb");
	if (!fp)
	{
		std::cout << "Failed to open file " << file_name << std::endl;
		return false;
	}

	for(size_t i = 0; i < _width * _height; i++) {
		RGBAPixel p = _pixels[i];
		char red = p.red;
		char green = p.green;
		char blue = p.blue;
		char alpha = p.alpha;

		fprintf(fp, "%c%c%c%c", red, green, blue, alpha);
		//std::cout << _pixels[i] << std::endl;
	}

	fclose(fp);

	return true;
}
