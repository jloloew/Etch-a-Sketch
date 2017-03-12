/**
 * @file image.h
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include "png.h"

class Image : public PNG
{
public:
	using PNG::PNG;

	/**
	*/
	void etchToFile(string const & file_name);
};

#endif // IMAGE_H
