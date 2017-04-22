//
//  EdgeDetector.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef EdgeDetector_hpp
#define EdgeDetector_hpp

#include "Image.hpp"

namespace etchasketch {
	namespace edgedetect {
		
		/**
		 * Finds edges within an image. @c EdgeDetector is an abstract
		 * superclass; do not attempt to instantiate it.
		 */
		class EdgeDetector {
		public:
			virtual ~EdgeDetector() { };
			
			/// Detect edges in an image.
			virtual etchasketch::Image *
			detectEdges(const etchasketch::Image &grayscaleImage) const = 0;
		};
		
	}
}

#endif /* EdgeDetector_hpp */
