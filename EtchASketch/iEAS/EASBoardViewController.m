//
//  EASBoardViewController.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASBoardViewController.h"

@interface EASBoardViewController ()

/**
 The view that's the exact size of the screen. Contains things like the
 @c EASScreenView.
 */
@property (weak, nonatomic) IBOutlet UIView *screenContents;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;

- (void)doComputationSequence;

@end

@implementation EASBoardViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	self.statusLabel.text = @"Loading…";
	
	// Set up image flow.
	EASImage *image;
	//*
	UIImage *img = [UIImage imageNamed:@"Lena"];
	image = [[EASImage alloc] initWithImage:img];
	/*/
	image = [EASImage imageFromEtchFileAtPath:@"/Users/jloloew/Desktop/EtchASketch/EtchASketch/EtchCLI/utils/lena.etch"];
	// */
	self.imageFlow = [[EASImageFlow alloc] initWithColorImage:image];
	self.imageFlow.delegate = self;
	
	// Set up screen VC.
	self.screenVC = [[EASScreenViewController alloc]
					 initWithNibName:@"EASScreenViewController" bundle:nil];
	self.screenVC.view.frame = self.screenContents.bounds;
	[self.screenContents addSubview:self.screenVC.view];
	[self.view bringSubviewToFront:self.statusLabel];
	
	[self addObserver:self
		   forKeyPath:@"screenVC.view.frame"
			  options:(NSKeyValueObservingOptions)(NSKeyValueObservingOptionNew | NSKeyValueObservingOptionInitial)
			  context:nil];
	
	// Wait for the UI to load, then begin computation.
	[NSTimer scheduledTimerWithTimeInterval:1.0
									 target:self
								   selector:@selector(doComputationSequence)
								   userInfo:nil
									repeats:NO];
}

- (void)viewDidDisappear:(BOOL)animated {
	[self removeObserver:self forKeyPath:@"screenVC.view.frame"];
	
	[super viewDidDisappear:animated];
}

- (void)doComputationSequence {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
		[self.imageFlow generateGrayscaleImage];
		
		[self.imageFlow detectEdges];
		
		[self.imageFlow generateEdgePoints];
		
		[self.imageFlow orderEdgePointsForDrawing];
		
		[self.imageFlow scalePointsToFitOutputSize];
		
//		NSArray<NSValue *> *orderedPoints = [self.imageFlow getFinalPoints];
//		NSLog(@"Ordered edge points: %@", orderedPoints);
	});
}

#pragma mark EASImageFlowDelegate

- (void)imageFlow:(EASImageFlow * __unused)imageFlow
willBeginComputationStage:(EASComputationStage)computationStage
{
	// Set the status label for the current stage of computation.
	NSString *statusText = nil;
	switch (computationStage) {
		case EASComputationStageNone:
			statusText = @"Loading…";
			break;
		case EASComputationStageGenerateGrayscaleImage:
			statusText = @"Generating grayscale image…";
			break;
		case EASComputationStageDetectEdges:
			statusText = @"Detecting edges…";
			break;
		case EASComputationStageGenerateEdgePoints:
			statusText = @"Generating edge points…";
			break;
		case EASComputationStageOrderEdgePointsForDrawing:
			statusText = @"Ordering edge points for drawing…";
			break;
		case EASComputationStageScalePointsToFitOutputSize:
			statusText = @"Scaling edge points to fit output size…";
			break;
		case EASComputationStageFinished:
			statusText = @"Done!";
			break;
	}
	// Log it and update the status label.
	NSLog(@"New status: %@", statusText);
	dispatch_sync(dispatch_get_main_queue(), ^{
		self.statusLabel.text = statusText;
	});
}

/*
- (void)imageFlow:(EASImageFlow *)imageFlow
didCompleteComputationStage:(EASComputationStage)computationStage
{
	// Get the image that was just produced.
	UIImage *producedImage = nil;
	switch (computationStage) {
		case EASComputationStageGenerateGrayscaleImage:
			producedImage = [imageFlow grayscaleImage];
			break;
		
		case EASComputationStageDetectEdges:
			producedImage = [imageFlow detectedEdgesImage];
			break;
		
	  default:
			break;
	}
	
	// Throw the image on the screen.
	[self displayProducedImage:producedImage];
}
// */

- (void)displayProducedImage:(UIImage *)image {
	if (!image) {
		return;
	}
	
	// Display the image.
	dispatch_sync(dispatch_get_main_queue(), ^{
		UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
		imageView.frame = [[UIScreen mainScreen] bounds];
		[self.view addSubview:imageView];
	});
}

- (void)imageFlowDidCompleteAllComputations:(EASImageFlow * __unused)imageFlow {
	// Draw the ordered edge points.
	NSArray<NSValue *> * __block points = [self.imageFlow getFinalPoints];
	dispatch_sync(dispatch_get_main_queue(), ^{
		[self.screenVC addPoints:points animated:YES];
	});
}

- (void)observeValueForKeyPath:(NSString *)keyPath
					  ofObject:(id)object
						change:(NSDictionary<NSKeyValueChangeKey,id> *)change
					   context:(void *)context
{
	if ([keyPath isEqualToString:@"screenVC.view.frame"] && [object isEqual:self]) {
		// Set the size of the drawn points.
		NSValue *newFrameValue = change[NSKeyValueChangeNewKey];
		CGSize newSize = [newFrameValue CGRectValue].size;
		[self.imageFlow setOutputSizeWithWidth:(NSUInteger)newSize.width
										height:(NSUInteger)newSize.height];
		if (self.imageFlow.computationStage == EASComputationStageFinished) {
			// Run it again.
			[self.imageFlow scalePointsToFitOutputSize];
		}
	} else {
		[super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
	}
}

@end
