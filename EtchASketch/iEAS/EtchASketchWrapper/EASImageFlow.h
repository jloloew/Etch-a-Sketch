//
//  EASImageFlow.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EASImage.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @typedef EASComputationStage
 * @brief A listing of stages of computation.
 */
typedef NS_ENUM(NSUInteger, EASComputationStage) {
	/// No computation is being performed.
	EASComputationStageNone = 0,
	
	/// The color image is being converted to grayscale.
	EASComputationStageGenerateGrayscaleImage,
	
	/// The grayscale image is undergoing edge detection.
	EASComputationStageDetectEdges,
	
	/// A list of edge points is being extracted from the edge detected image.
	EASComputationStageGenerateEdgePoints,
	
	/**
	 * The list of points to be drawn is being put in the order in which they
	 * should be drawn.
	 */
	EASComputationStageOrderEdgePointsForDrawing,
	
	/// The points in drawing order are being scaled to fit the output size.
	EASComputationStageScalePointsToFitOutputSize,
	
	/// All computations are complete.
	EASComputationStageFinished
};

@class EASImageFlow;

/// Receives callbacks when each stage of computation completes.
@protocol EASImageFlowDelegate <NSObject>

/**
 * Called when the image flow is about to begin another stage of computation.
 *
 * @param imageFlow The image flow that will begin a computation stage.
 * @param computationStage The stage of computation that will be run.
 */
@optional
- (void)imageFlow:(EASImageFlow *)imageFlow willBeginComputationStage:(EASComputationStage)computationStage;

/**
 * Called when the image flow has completed another stage of computation.
 *
 * @note This method is not called for @c EASComputationStageFinished.
 * 
 * @param imageFlow The image flow that has just completed a computation stage.
 * @param computationStage The stage of computation that has just finished.
 */
- (void)imageFlow:(EASImageFlow *)imageFlow didCompleteComputationStage:(EASComputationStage)computationStage;

/**
 * Called when the image flow has completed all stages of computation.
 * 
 * @param imageFlow The image flow that has just completed all computations.
 */
- (void)imageFlowDidCompleteAllComputations:(EASImageFlow *)imageFlow;

@end


/**
 * The top-level object that takes an image and walks it through each stage of
 * processing. It's a wrapper around the C++ @c etchasketch::ImageFlow.
 */
@interface EASImageFlow : NSObject

/// The current stage of computation.
@property (atomic, readonly) EASComputationStage computationStage;

/**
 * The delegate to receive callbacks when the computation stage progresses or
 * regresses.
 */
@property (atomic, weak, nullable) id<EASImageFlowDelegate> delegate;

/// Create a new flow with a starting image.
- (instancetype)initWithColorImage:(EASImage *)colorImage;

/// Create a new flow with a starting image and desired output size.
- (instancetype)initWithColorImage:(EASImage *)colorImage
					   outputWidth:(NSUInteger)outputWidth
					  outputHeight:(NSUInteger)outputHeight;

/// Convert the color image to a grayscale image.
- (void)generateGrayscaleImage;

/// Detect edges in the starting image.
- (void)detectEdges;

/// Get a set of all points on an edge in the edge detected image.
- (void)generateEdgePoints;

/// Put the edge points in the best order for drawing.
- (void)orderEdgePointsForDrawing;

/// Scale the ordered points up or down to fit in the desired output frame.
- (void)scalePointsToFitOutputSize;

/// Get the points to draw in order, first computing them if necessary.
- (NSArray<NSValue *> *)getFinalPoints;

/// Perform the entire computation sequence.
- (void)performAllComputationSteps;

/// Set the desired output resolution. Units are in pixels.
- (void)setOutputSizeWithWidth:(NSUInteger)width height:(NSUInteger)height;

#pragma mark Image getters

/// Create a grayscale image.
- (nullable UIImage *)grayscaleImage;

/// Create an image containing the detected edges.
- (nullable UIImage *)detectedEdgesImage;

@end
NS_ASSUME_NONNULL_END
