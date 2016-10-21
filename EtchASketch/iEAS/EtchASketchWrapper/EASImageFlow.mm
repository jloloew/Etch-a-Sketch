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

- (void)detectEdges {
	NSAssert(self.computationStage == EASComputationStageNone, @"Computation already completed");
	self.computationStage = EASComputationStageDetectEdges;
	self.imageFlow->detectEdges();
	
	// Notify the delegate.
	if (self.delegate && [self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (void)generateEdgePoints {
	self.imageFlow->generateEdgePoints();
	
	// Notify the delegate.
	if (self.delegate && [self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (void)orderEdgePointsForDrawing {
	self.imageFlow->orderEdgePointsForDrawing();
	
	// Notify the delegate.
	if (self.delegate && [self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
		[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
	}
}

- (NSArray<NSValue *> *)getOrderedEdgePoints {
	const vector<KDPoint<2>> &pts = self.imageFlow->getOrderedEdgePoints();
	// Copy the data out, converting to NSValue.
	NSMutableArray<NSValue *> *points = [NSMutableArray arrayWithCapacity:pts.size()];
	for (auto it = pts.begin(); it != pts.end(); ++it) {
		[points addObject:[NSValue valueWithCGPoint:CGPointMake((*it)[0], (*it)[1])]];
	}
	return points;
}

@end
