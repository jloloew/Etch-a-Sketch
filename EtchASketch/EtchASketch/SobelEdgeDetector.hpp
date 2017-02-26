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

		class SobelEdgeDetector : public etchasketch::edgedetect::EdgeDetector {
		public:
			SobelEdgeDetector();
			
			virtual ~SobelEdgeDetector() { };

			/**
			 * Detect edges in an image, copying the result into a newly
			 * allocated image.
			 */
			virtual etchasketch::Image *
			detectEdges(const etchasketch::Image &grayscaleImage) const;

		private:
			float intensityForPoint(const etchasketch::Image &img,
									const int x,
									const int y) const;

		};

	}
}

#endif /* SobelEdgeDetector_hpp */
