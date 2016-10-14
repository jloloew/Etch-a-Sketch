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
	
	/**
	 * The backing store of a single image.
	 */
	struct Image {
	public:
		/// RGBA format.
		typedef uint32_t Pixel;
		
		Image(size_t width, size_t height, const Pixel *data = nullptr);
		
		// Deep copy another image.
		Image(const etchasketch::Image &other);
		
		virtual ~Image();
		
		inline
		bool isValid() const {
			return (getWidth() > 0) && (getHeight() > 0) && (nullptr != data);
		}
		
		inline
		size_t getWidth() const {
			return width;
		}
		
		inline
		size_t getHeight() const {
			return height;
		}
		
		Pixel operator[] (const etchasketch::KDPoint<2> &index) const;
		
		Pixel & operator[] (const etchasketch::KDPoint<2> &index);
		
	private:
		size_t width, height;
		
		Pixel *data;
		
		size_t getPixelCount() const
			// TODO: check for overflow on the multiplication
			{ return width * height; }
	};
	
}

#endif /* Image_hpp */
