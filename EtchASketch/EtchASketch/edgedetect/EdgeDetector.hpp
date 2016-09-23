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
using etchasketch::Image;

namespace etchasketch {
	namespace edgedetect {
		
		class EdgeDetector {
		public:
			EdgeDetector();
			
			/// Detect edges in an image.
			virtual Image * detectEdges(const Image &grayscaleImage) const;
			
		};
		
	}
}

#endif /* EdgeDetector_hpp */
