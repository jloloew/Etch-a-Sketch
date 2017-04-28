//
//  Image.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <stdint.h>
#include "KDPoint.hpp"

namespace etchasketch {

/// The backing store of a single image.
struct Image {
  public:
	/// A pixel in RGBA format.
	typedef uint32_t Pixel;

	// TODO: Add spec for the format of the data buffer.
	/**
	 * Create a new image.
	 *
	 * @param width The width of the image, in pixels.
	 * @param height The height of the image, in pixels.
	 * @param data A raw pixel buffer. The data is copied out into the
	 * Image's own internal buffer.
	 */
	Image(size_t width, size_t height, const Pixel *data = nullptr);

	/// Deep copy another image.
	Image(const etchasketch::Image &other);
	
	/// Create a copy of another image, resizing in the process.
	Image(const etchasketch::Image &other, size_t newWidth, size_t newHeight);
	
	Image & operator=(const Image &that);

	virtual ~Image();
	
	/**
	 * Change the size of an image, scaling the image's current contents if
	 * necessary.
	 */
	void resize(size_t newWidth, size_t newHeight);

	/// Whether this is a valid, visually representable image.
	inline bool isValid() const
		{ return (getWidth() > 0) && (getHeight() > 0) && (nullptr != data); }

	/// The width of the image, in pixels.
	inline size_t getWidth() const
		{ return width; }

	/// The height of the image, in pixels.
	inline size_t getHeight() const
		{ return height; }

	/// Get the @c Pixel at the given point.
	Pixel operator[](const etchasketch::KDPoint<2> &index) const;
	
	/// Get the @c Pixel at the given point.
	Pixel &operator[](const etchasketch::KDPoint<2> &index);

	// For the Objective-C wrapper.

	/// Get a pointer to the data buffer used by this image.
	inline Pixel *getData() const
		{ return data; }

	/// Get the number of pixels in the image.
	inline size_t getPixelCount() const
	// TODO: check for overflow on the multiplication
		{ return width * height; }

  private:
	/// The width of the image, in pixels.
	size_t width;
	
	/// The height of the image, in pixels.
	size_t height;

	/**
	 * 1-D array of Pixels representing a 2-D image. See @c operator[] for
	 * mapping.
	 */
	Pixel *data;
};
}

#endif /* Image_hpp */
