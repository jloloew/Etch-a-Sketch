/**
 * @file image.cpp
 * @author jloew2
 */

#include "image.h"
#include <iostream>

void Image::etchToFile(string const & file_name) {
	FILE * fp = fopen(file_name.c_str(), "wb");
	if (!fp)
	{
		std::cout << "Failed to open file " << file_name << std::endl;
		return;
	}

    fwrite(_pixels, sizeof(RGBAPixel), _width * _height, fp);
	fclose(fp);
}
