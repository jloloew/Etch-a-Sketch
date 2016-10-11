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

//using etchasketch::KDPoint;

namespace etchasketch {
	
//	// Forward declare.
//	template<int Dim>
//	class KDPoint;
	
	/**
	 * The backing store of a single image.
	 */
	class Image {
	public:
		/// RGBA format.
		typedef uint32_t Pixel;
		
		Image(size_t width, size_t height);
		
		Image(const Pixel *data, size_t width, size_t height);
		
		virtual ~Image();
		
		size_t getWidth(void) const;
		
		size_t getHeight(void) const;
		
		const Pixel & operator[] (const etchasketch::KDPoint<2> & index) const;
		
		Pixel & operator[] (const etchasketch::KDPoint<2> & index);
		
	private:
		size_t width, height;
		
		Pixel *data;
	};
	
}

#endif /* Image_hpp */
