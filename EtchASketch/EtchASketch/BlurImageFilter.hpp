//
//  BlurImageFilter.hpp
//  EtchASketch
//
//  Created by Justin Loew on 12/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef BlurImageFilter_hpp
#define BlurImageFilter_hpp

#include "ImageFilter.hpp"

namespace etchasketch {
	namespace edgedetect {
		
		/// An image filter that applies a blur.
		class BlurImageFilter : public etchasketch::edgedetect::ImageFilter {
		public:
			/// The radius of the blur.
			static const uint32_t radius = 4;
			
			BlurImageFilter();
			
			virtual ~BlurImageFilter() { };
			
			/**
			 * Apply a Gaussian blur to the image, copying the result into a
			 * newly allocated image.
			 */
			virtual etchasketch::Image *
			apply(const etchasketch::Image &originalImage) const;
			
		private:
			static const uint32_t kernelSize = 2*radius + 1;
			float kernel[kernelSize][kernelSize];
			
			void initKernel();
			
			/**
			 * Compute the blurred version of a pixel by examining the 
			 * surrounding pixels.
			 */
			etchasketch::Image::Pixel
			blurredPixel(const etchasketch::Image &img, int x, int y) const;
		};
		
	}
}

#endif /* BlurImageFilter_hpp */
