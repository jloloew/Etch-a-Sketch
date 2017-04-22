//
//  EASImageFlow.mm
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImage+CPP.hh"
#import "EASImageFlow.h"
#import "EtchASketch.hpp"

using etchasketch::KDPoint;

NS_ASSUME_NONNULL_BEGIN
@interface EASImageFlow ()

@property (atomic, readwrite) EASComputationStage computationStage;

@property (nonatomic, readonly, nonnull) etchasketch::ImageFlow *imageFlow;

/// The serial queue on which work is synchronously run.
@property (nonatomic, readonly, nonnull) dispatch_queue_t processingQueue;

/**
 * The serial queue on which delegate callbacks are asynchronously run.
 * This is just the global dispatch queue, at least for now.
 */
@property (nonatomic, readonly, nonnull) dispatch_queue_t delegateQueue;

@end
NS_ASSUME_NONNULL_END


@implementation EASImageFlow

- (instancetype)initWithColorImage:(EASImage *)colorImage {
	self = [super init];
	if (self != nil) {
		self.computationStage = EASComputationStageNone;
		self.delegate = nil;
		_imageFlow = new etchasketch::ImageFlow(*colorImage.image);
		// Set up the dispatch queue.
		dispatch_queue_attr_t attr = dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INITIATED, -1);
		_processingQueue = dispatch_queue_create("com.justinloew.EASImageFlow.processing", attr);
		_delegateQueue = dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0);
	}
	return self;
}

- (void)dealloc {
	delete self.imageFlow;
}

#pragma mark Computations

- (void)generateGrayscaleImage {
	dispatch_sync(self.processingQueue, ^{
		NSAssert(self.computationStage == EASComputationStageNone, @"Computation already completed");
		if (self.computationStage >= EASComputationStageGenerateGrayscaleImage) {
			// Already done, or at least in progress.
			return;
		}
		
		dispatch_sync(self.delegateQueue, ^{
			// Update the current stage of computation and notify the delegate.
			self.computationStage = EASComputationStageGenerateGrayscaleImage;
			if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
				[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
			}
		});
		
		self.imageFlow->convertToGrayscale();
	});
	
	dispatch_async(self.delegateQueue, ^{
		// Notify the delegate.
		if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
			[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
		}
	});
}

- (void)detectEdges {
	dispatch_sync(self.processingQueue, ^{
		NSAssert(self.computationStage == EASComputationStageGenerateGrayscaleImage, @"Computation already completed");
		if (self.computationStage >= EASComputationStageDetectEdges) {
			// Already done, or at least in progress.
			return;
		}
		
		dispatch_sync(self.delegateQueue, ^{
			// Update the current stage of computation and notify the delegate.
			self.computationStage = EASComputationStageDetectEdges;
			if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
				[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
			}
		});
		
		self.imageFlow->detectEdges();
	});
	
	dispatch_async(self.delegateQueue, ^{
		// Notify the delegate.
		if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
			[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
		}
	});
}

- (void)generateEdgePoints {
	dispatch_sync(self.processingQueue, ^{
		NSAssert(self.computationStage == EASComputationStageDetectEdges, @"Computation already completed");
		if (self.computationStage >= EASComputationStageGenerateEdgePoints) {
			// Already done, or at least in progress.
			return;
		}
		
		dispatch_sync(self.delegateQueue, ^{
			// Update the current stage of computation and notify the delegate.
			self.computationStage = EASComputationStageGenerateEdgePoints;
			if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
				[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
			}
		});
		
		self.imageFlow->generateEdgePoints();
	});
	
	dispatch_async(self.delegateQueue, ^{
		// Notify the delegate.
		if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
			[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
		}
	});
}

- (void)orderEdgePointsForDrawing {
	dispatch_sync(self.processingQueue, ^{
		NSAssert(self.computationStage == EASComputationStageGenerateEdgePoints, @"Computation already completed");
		if (self.computationStage >= EASComputationStageOrderEdgePointsForDrawing) {
			// Already done, or at least in progress.
			return;
		}
		
		dispatch_sync(self.delegateQueue, ^{
			// Update the current stage of computation and notify the delegate.
			self.computationStage = EASComputationStageOrderEdgePointsForDrawing;
			if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
				[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
			}
		});
		
		self.imageFlow->orderEdgePointsForDrawing();
	});
	
	dispatch_async(self.delegateQueue, ^{
		// Notify the delegate.
		if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
			[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
		}
	});
}

- (void)scalePointsToFitOutputSize {
	dispatch_sync(self.processingQueue, ^{
		// We don't care if we've already completed this computation stage, since we
		// may have changed the output size.
			
		dispatch_sync(self.delegateQueue, ^{
			// Update the current stage of computation and notify the delegate.
			self.computationStage = EASComputationStageScalePointsToFitOutputSize;
			if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
				[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
			}
		});
		
		self.imageFlow->scalePointsToFitOutputSize();
	});
	
	dispatch_async(self.delegateQueue, ^{
		// Notify the delegate.
		if ([self.delegate respondsToSelector:@selector(imageFlow:didCompleteComputationStage:)]) {
			[self.delegate imageFlow:self didCompleteComputationStage:self.computationStage];
		}
		
		// This was the last stage of computation. We're done!
		if ([self.delegate respondsToSelector:@selector(imageFlowDidCompleteAllComputations:)]) {
			[self.delegate imageFlowDidCompleteAllComputations:self];
		}
	});
}

- (NSArray<NSValue *> *)getFinalPoints {
	NSMutableArray<NSValue *> * __block points = nil;
	dispatch_sync(self.processingQueue, ^{
		dispatch_sync(self.delegateQueue, ^{
			if (self.computationStage == EASComputationStageScalePointsToFitOutputSize) {
				// Update the current stage of computation and notify the delegate.
				self.computationStage = EASComputationStageFinished;
				if ([self.delegate respondsToSelector:@selector(imageFlow:willBeginComputationStage:)]) {
					[self.delegate imageFlow:self willBeginComputationStage:self.computationStage];
				}
			}
		});
		
		const std::vector<KDPoint<2>> &pts = self.imageFlow->getFinalPoints();
		NSUInteger numPts = (NSUInteger)pts.size();
		// Copy the data out, converting to NSValue.
		points = [NSMutableArray arrayWithCapacity:numPts];
		NSUInteger nonZero = 0;
		for (auto it = pts.begin(); it != pts.end(); ++it) {
			NSValue *value = [NSValue valueWithCGPoint:CGPointMake((*it)[0], (*it)[1])];
			if (!CGPointEqualToPoint(value.CGPointValue, CGPointZero)) {
				nonZero++;
			}
			[points addObject:value];
		}
	});
	return points;
}

- (void)performAllComputationSteps {
	[self generateGrayscaleImage];
	[self detectEdges];
	[self generateEdgePoints];
	[self orderEdgePointsForDrawing];
	[self scalePointsToFitOutputSize];
}

- (void)setOutputSizeWithWidth:(NSUInteger)width height:(NSUInteger)height {
	dispatch_sync(self.processingQueue, ^{
		self.imageFlow->setOutputSize(width, height);
	});
}

#pragma mark - Image getters

- (UIImage *)grayscaleImage {
	UIImage * __block grayImage = nil;
	dispatch_sync(self.processingQueue, ^{
		const etchasketch::Image & grayscaleImage = self.imageFlow->getGrayscaleImage();
		EASImage *easImage = [[EASImage alloc] initWithCPPImage:&grayscaleImage];
		grayImage = [easImage UIImage];
	});
	return grayImage;
}

- (UIImage *)detectedEdgesImage {
	UIImage * __block edgeImage = nil;
	dispatch_sync(self.processingQueue, ^{
		const etchasketch::Image & detectedEdgesImage = self.imageFlow->getEdgeDetectedImage();
		EASImage *easImage = [[EASImage alloc] initWithCPPImage:&detectedEdgesImage];
		edgeImage = [easImage UIImage];
	});
	return edgeImage;
}

@end
