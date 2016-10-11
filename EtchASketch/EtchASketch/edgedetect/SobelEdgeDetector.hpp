//
//  SobelEdgeDetector.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef SobelEdgeDetector_hpp
#define SobelEdgeDetector_hpp

#include "EdgeDetector.hpp"

namespace etchasketch {
	namespace edgedetect {
		
		class SobelEdgeDetector : EdgeDetector {
		public:
			SobelEdgeDetector();
			
			/// Detect edges in an image.
			virtual Image * detectEdges(const Image &grayscaleImage) const;
			
		private:
			float
			intensityForKDPoint(const Image &img,
							  const size_t x,
							  const size_t y) const;
			
		};
		
	}
}

#endif /* SobelEdgeDetector_hpp */
