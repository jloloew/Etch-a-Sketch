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
	 * The top-level object that takes an image and walks it through each stage
	 * of processing.
	 */
	class ImageFlow {
		
	public:
		
		/**
		 * Create a new flow with a starting image.
		 */
		ImageFlow(const etchasketch::Image &colorImage);
		
		virtual ~ImageFlow();
		
		/*
		 * These public methods are listed in the order in which they are 
		 * intended to be called.
		 */
		
		/**
		 * Convert the color image into a grayscale image.
		 */
		void convertToGrayscale();
		
		/**
		 * Detect edges in the starting image.
		 */
		void detectEdges();
		
		/**
		 * Get a set of all points on an edge in the edge detected image.
		 */
		void generateEdgePoints();
		
		/**
		 * Put the edge points in the best order for drawing.
		 */
		void orderEdgePointsForDrawing();
		
		/**
		 * Get the points in drawing order, generating them if necessary.
		 */
		const std::vector<etchasketch::KDPoint<2>> & getOrderedEdgePoints();
		
		// For the Objective-C wrapper.
		
		const etchasketch::Image & getGrayscaleImage() const
			{ return grayscaleImage; }
		
		const etchasketch::Image * getEdgeDetectedImage() const
			{ return edgeDetectedImage; }
		
	private:
		// Images and other such things, in order of use.
		const etchasketch::Image originalImage;
		etchasketch::Image grayscaleImage;
		const etchasketch::Image *edgeDetectedImage;
		const std::unordered_set<etchasketch::KDPoint<2>> *edgePoints;
		const std::vector<etchasketch::KDPoint<2>> *orderedEdgePoints;
		
		etchasketch::edgedetect::EdgeDetector *edgeDetector;
		etchasketch::salesman::Salesman *salesman;
		
		// Setters
		void setEdgeDetectedImage(const etchasketch::Image *newImage);
		
		void setEdgePoints(const std::unordered_set<etchasketch::KDPoint<2>>
						   *newEdgePoints);
		
		void setOrderedEdgePoints(const std::vector<etchasketch::KDPoint<2>>
								  *newOrderedEdgePoints);
		
		void setSalesman(etchasketch::salesman::Salesman *newSalesman);
		
	};
	
}

#endif /* ImageFlow_hpp */
