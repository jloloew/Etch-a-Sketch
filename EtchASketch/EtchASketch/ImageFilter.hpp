//
//  ImageFilter.hpp
//  EtchASketch
//
//  Created by Justin Loew on 12/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef ImageFilter_hpp
#define ImageFilter_hpp

#include "Image.hpp"

namespace etchasketch {
	namespace edgedetect {
		
		/// Abstract class. Apply a filter to an image.
		class ImageFilter {
		public:
			virtual ~ImageFilter() { };
			
			/// Apply the filter to the image.
			virtual etchasketch::Image *
			apply(const etchasketch::Image &originalImage) const = 0;
		};
		
	}
}

#endif /* ImageFilter_hpp */
