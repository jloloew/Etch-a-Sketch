//
//  EASImageFlowManager.mm
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImageFlowManager.h"
#import "EtchASketch.hpp"

using etchasketch::Image;
using etchasketch::ImageFlowManager;

@interface EASImageFlowManager ()

@property (nonatomic, readwrite) EASComputationStage computationStage;

@property (nonatomic, readonly) ImageFlowManager *imageFlowManager;

@end


@interface EASImage ()

@property (nonatomic, readonly) Image *image;

- (instancetype)initWithCPPImage:(const etchasketch::Image *)image;

@end


@implementation EASImageFlowManager

- (instancetype)initWithColorImage:(EASImage *)colorImage {
	self = [super init];
	if (self != nil) {
		self.computationStage = EASComputationStageNone;
		self.delegate = nil;
		_imageFlowManager = new ImageFlowManager(*colorImage.image);
	}
	return self;
}

- (void)dealloc {
	delete self.imageFlowManager;
	_imageFlowManager = nullptr;
}

#pragma mark Computations

- (void)generateGrayscaleImage {
	NSAssert(self.computationStage == EASComputationStageNone, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageGenerateGrayscaleImage;
	if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	self.imageFlowManager->convertToGrayscale();
	
	// Notify the delegate.
	if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (void)detectEdges {
	NSAssert(self.computationStage == EASComputationStageGenerateGrayscaleImage, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageDetectEdges;
	if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	self.imageFlowManager->detectEdges();
	
	// Notify the delegate.
	if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (void)generateEdgePoints {
	NSAssert(self.computationStage == EASComputationStageDetectEdges, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageGenerateEdgePoints;
	if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	self.imageFlowManager->generateEdgePoints();
	
	// Notify the delegate.
	if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (void)orderEdgePointsForDrawing {
	NSAssert(self.computationStage == EASComputationStageGenerateEdgePoints, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageOrderEdgePointsForDrawing;
	if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	self.imageFlowManager->orderEdgePointsForDrawing();
	
	// Notify the delegate.
	if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
	
	// This was the last stage of computation. We're done!
	if ([self.delegate respondsToSelector:@selector(imageFlowDidCompleteAllComputations:)]) {
		[self.delegate imageFlowDidCompleteAllComputations:self];
	}
}

- (NSArray<NSValue *> *)getOrderedEdgePoints {
	if (self.computationStage == EASComputationStageOrderEdgePointsForDrawing) {
		// Update the current stage of computation and notify the delegate.
		self.computationStage = EASComputationStageFinished;
		if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
			[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
		}
	}
	
	const vector<KDPoint<2>> &pts = self.imageFlowManager->getOrderedEdgePoints();
	NSUInteger numPts = (NSUInteger)pts.size();
	// Copy the data out, converting to NSValue.
	NSMutableArray<NSValue *> *points = [NSMutableArray arrayWithCapacity:numPts];
	for (auto it = pts.begin(); it != pts.end(); ++it) {
		[points addObject:[NSValue valueWithCGPoint:CGPointMake((*it)[0], (*it)[1])]];
	}
	return points;
}

#pragma mark - Image getters

- (UIImage *)grayscaleImage {
	const Image & grayscaleImage = self.imageFlowManager->getGrayscaleImage();
	EASImage *easImage = [[EASImage alloc] initWithCPPImage:&grayscaleImage];
	UIImage *grayImage = [easImage UIImage];
	return grayImage;
}

- (UIImage *)detectedEdgesImage {
	const Image *detectedEdgesImage = self.imageFlowManager->getEdgeDetectedImage();
	EASImage *easImage = [[EASImage alloc] initWithCPPImage:detectedEdgesImage];
	UIImage *edgeImage = [easImage UIImage];
	return edgeImage;
}

@end
