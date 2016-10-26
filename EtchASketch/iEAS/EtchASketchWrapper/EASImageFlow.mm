//
//  EASImageFlow.mm
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImageFlow.h"
#import "EtchASketch.hpp"

using etchasketch::Image;
using etchasketch::ImageFlow;

@interface EASImageFlow ()

@property (nonatomic, readwrite) EASComputationStage computationStage;

@property (nonatomic, readonly) ImageFlow *imageFlow;

@end


@interface EASImage ()

@property (nonatomic, readonly) Image *image;

- (instancetype)initWithCPPImage:(const etchasketch::Image *)image;

@end


@implementation EASImageFlow

- (instancetype)initWithColorImage:(EASImage *)colorImage {
	self = [super init];
	if (self != nil) {
		self.computationStage = EASComputationStageNone;
		self.delegate = nil;
		_imageFlow = new ImageFlow(*colorImage.image);
	}
	return self;
}

- (void)dealloc {
	delete self.imageFlow;
	_imageFlow = nullptr;
}

#pragma mark Computations

- (void)generateGrayscaleImage {
	NSAssert(self.computationStage == EASComputationStageNone, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageGenerateGrayscaleImage;
	if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	self.imageFlow->convertToGrayscale();
	
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
	
	self.imageFlow->detectEdges();
	
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
	
	self.imageFlow->generateEdgePoints();
	
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
	
	self.imageFlow->orderEdgePointsForDrawing();
	
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
	NSAssert(self.computationStage == EASComputationStageOrderEdgePointsForDrawing, @"Computation already completed");
	// Update the current stage of computation and notify the delegate.
	self.computationStage = EASComputationStageFinished;
	if (self.delegate && [self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
		[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
	}
	
	const vector<KDPoint<2>> &pts = self.imageFlow->getOrderedEdgePoints();
	// Copy the data out, converting to NSValue.
	NSMutableArray<NSValue *> *points = [NSMutableArray arrayWithCapacity:pts.size()];
	for (auto it = pts.begin(); it != pts.end(); ++it) {
		[points addObject:[NSValue valueWithCGPoint:CGPointMake((*it)[0], (*it)[1])]];
	}
	return points;
}

- (UIImage *)grayscaleImage {
	const Image & grayscaleImage = self.imageFlow->getGrayscaleImage();
	EASImage *easImage = [[EASImage alloc] initWithCPPImage:&grayscaleImage];
	UIImage *grayImage = [easImage UIImage];
	return grayImage;
}

@end
