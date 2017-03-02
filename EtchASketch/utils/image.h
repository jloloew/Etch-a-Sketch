/**
 * @file image.h
 * @author jloew2
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
	 * Flips the image about a vertical line through its center by swapping pixels.
	 */
	void flipleft();

	/**
	 * Adds to the red, green, and blue parts of each pixel in the image.
	 */
	void adjustbrightness(int r, int g, int b);

	/**
	 * Makes each RGB component of each pixel equal to 255 minus its original value.
	 */
	void invertcolors();

	/**
	*/
	bool etchToFile(string const & file_name);
};

#endif // IMAGE_H
