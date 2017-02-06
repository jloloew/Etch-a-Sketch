//
//  EASImageFlowManager.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EASImage.h"

NS_ASSUME_NONNULL_BEGIN
typedef NS_ENUM(NSUInteger, EASComputationStage) {
	EASComputationStageNone = 0,
	EASComputationStageGenerateGrayscaleImage,
	EASComputationStageDetectEdges,
	EASComputationStageGenerateEdgePoints,
	EASComputationStageOrderEdgePointsForDrawing,
	EASComputationStageFinished
};

@class EASImageFlowManager;

/// Used to receive callbacks when each stage of computation completes.
@protocol EASImageFlowManagerDelegate <NSObject>

@optional
- (void)imageFlow:(EASImageFlowManager *)imageFlow willBeginComputationStage:(EASComputationStage)computationStage;

// Not called for @c EASComputationStageFinished.
- (void)imageFlow:(EASImageFlowManager *)imageFlow didCompleteComputationStage:(EASComputationStage)computationStage;

- (void)imageFlowDidCompleteAllComputations:(EASImageFlowManager *)imageFlow;

@end

/**
 * The top-level object that takes an image and walks it through each stage of
 * processing.
 */
@interface EASImageFlowManager : NSObject

@property (nonatomic, readonly) EASComputationStage computationStage;

@property (nonatomic, weak, nullable) id<EASImageFlowManagerDelegate> delegate;

/// Create a new flow with a starting image.
- (instancetype)initWithColorImage:(EASImage *)colorImage;

/// Convert the color image to a grayscale image.
- (void)generateGrayscaleImage;

/// Detect edges in the starting image.
- (void)detectEdges;

/// Get a set of all points on an edge in the edge detected image.
- (void)generateEdgePoints;

/// Put the edge points in the best order for drawing.
- (void)orderEdgePointsForDrawing;

/// Get the points to draw in order, first computing them if necessary.
- (NSArray<NSValue *> *)getOrderedEdgePoints;

#pragma mark Image getters

- (nullable UIImage *)grayscaleImage;
- (nullable UIImage *)detectedEdgesImage;

@end
NS_ASSUME_NONNULL_END
