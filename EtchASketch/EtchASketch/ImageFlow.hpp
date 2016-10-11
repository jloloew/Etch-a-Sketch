//
//  ImageFlow.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef ImageFlow_hpp
#define ImageFlow_hpp

#include <unordered_set>
#include <vector>
#include "Image.hpp"
#include "EdgeDetector.hpp"
#include "Salesman.hpp"

namespace etchasketch {
	
	/**
	 * The top-level object that takes an image and walks it through each stage of processing.
	 */
	class ImageFlow {
		
	public:
		
		// These public methods are listed in the order in which they are intended to be called.
		
		/**
		 * Create a new flow with a starting image.
		 */
		ImageFlow(const Image &colorImage);
		
		virtual ~ImageFlow(void);
		
		/**
		 * Detect edges in the starting image.
		 */
		void detectEdges(void);
		
		/**
		 * Get a set of all points on an edge in the edge detected image.
		 */
		void generateEdgePoints(void);
		
		/**
		 * Put the edge points in the best order for drawing.
		 */
		void orderEdgePointsForDrawing(void);
		
	private:
		// Images and other such things, in order of use.
		const Image originalImage;
		Image grayscaleImage;
		Image *edgeDetectedImage;
		const std::unordered_set<KDPoint<2> *> *edgePoints;
		const std::vector<KDPoint<2>> *orderedEdgePoints;
		
		edgedetect::EdgeDetector edgeDetector;
		salesman::Salesman *salesman;
		
		void convertToGrayscale(void);
		
	};
	
}

#endif /* ImageFlow_hpp */
