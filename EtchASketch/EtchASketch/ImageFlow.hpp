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
		
		/**
		 * Create a new flow with a starting image and desired output size.
		 */
		ImageFlow(const etchasketch::Image &colorImage,
				  size_t outputWidth,
				  size_t outputHeight);
		
		virtual ~ImageFlow();
		
		/*
		 * These public methods are listed in the order in which they are 
		 * intended to be called.
		 */
		
		/// Convert the color image into a grayscale image.
		void convertToGrayscale();
		
		/// Detect edges in the starting image.
		void detectEdges();
		
		/// Get a set of all points on an edge in the edge detected image.
		void generateEdgePoints();
		
		/// Put the edge points in the best order for drawing.
		void orderEdgePointsForDrawing();
		
		/**
		 * Scale the ordered edge points to fit the output size. May stretch the
		 * image so it doesn't scale evenly.
		 */
		void scalePointsToFitOutputSize();
		
		/// Get the points in drawing order, generating them if necessary.
		const std::vector<etchasketch::KDPoint<2>> & getFinalPoints();
		
		/// Do the entire computation flow.
		void performAllComputationSteps();
		
		/// Set the desired output resolution.
		void setOutputSize(size_t width, size_t height);
		
		// For the Objective-C wrapper.
		
		/// Get the grayscale image, if we've already produced it.
		const etchasketch::Image & getGrayscaleImage() const
			{ return grayscaleImage; }
		
		/// Get the edge detected image, if we've already produced it.
		const etchasketch::Image & getEdgeDetectedImage() const
			{ return edgeDetectedImage; }
		
	private:
		const etchasketch::Image fullSizeOriginalImage;
		/// The desired width of the ordered points, in pixels.
		size_t outputWidth;
		
		/// The desired height of the ordered points, in pixels.
		size_t outputHeight;
		
		// Images and other such things, in order of use.
		etchasketch::Image originalImage;
		etchasketch::Image grayscaleImage;
		etchasketch::Image edgeDetectedImage;
		const std::unordered_set<etchasketch::KDPoint<2>> *edgePoints;
		const std::vector<etchasketch::KDPoint<2>> *orderedEdgePoints;
		const std::vector<etchasketch::KDPoint<2>> *scaledEdgePoints;
		
		etchasketch::edgedetect::EdgeDetector *edgeDetector;
		etchasketch::salesman::Salesman *salesman;
		
		const etchasketch::Image
		downscaleOriginalImageIfNecessary(const Image &fullImage) const;
		
		// Setters
		void setEdgePoints(const std::unordered_set<etchasketch::KDPoint<2>>
						   *newEdgePoints);
		
		void setOrderedEdgePoints(const std::vector<etchasketch::KDPoint<2>>
								  *newOrderedEdgePoints);
		
		void setScaledEdgePoints(const std::vector<etchasketch::KDPoint<2>>
								 *newScaledEdgePoints);
		
		void setSalesman(etchasketch::salesman::Salesman *newSalesman);
		
	};
	
}

#endif /* ImageFlow_hpp */
