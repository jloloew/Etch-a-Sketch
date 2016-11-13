//
//  EASBoardViewController.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASBoardViewController.h"

@interface EASBoardViewController ()

/// The view that's the exact size of the screen. Contains things like the @c EASScreenView.
@property (weak, nonatomic) IBOutlet UIView *screenContents;
@property (weak, nonatomic) IBOutlet UILabel *statusLabel;

- (void)doComputationSequence;

@end

@implementation EASBoardViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	self.statusLabel.text = @"Loading…";
	
	// Set up image flow.
	UIImage *img = [UIImage imageNamed:@"Lena"];
	EASImage *image = [[EASImage alloc] initWithImage:img];
	self.imageFlow = [[EASImageFlow alloc] initWithColorImage:image];
	self.imageFlow.delegate = self;
	
	// Set up screen VC.
	self.screenVC = [[EASScreenViewController alloc] initWithNibName:@"EASScreenViewController" bundle:nil];
	self.screenVC.view.frame = self.screenContents.frame;
	[self.screenContents addSubview:self.screenVC.view];
	
	// Wait for the UI to load, then begin computation.
	[NSTimer scheduledTimerWithTimeInterval:1.0
									 target:self
								   selector:@selector(doComputationSequence)
								   userInfo:nil
									repeats:NO];
}

- (void)doComputationSequence {
	//*
	// Don't forget to uncomment imageFlow:didCompleteComputationStage:
	[self test_doComputationSequence];
	return;
	/*/
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
		[self.imageFlow generateGrayscaleImage];
		
		[self.imageFlow detectEdges];
		
		[self.imageFlow generateEdgePoints];
		
		[self.imageFlow orderEdgePointsForDrawing];
	});
	// */
}

- (void)test_doComputationSequence {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
		[self.imageFlow generateGrayscaleImage];
		[self.imageFlow detectEdges];
	});
}

#pragma mark EASImageFlowDelegate

- (void)imageFlow:(EASImageFlow * __unused)imageFlow willBeginComputationStage:(EASComputationStage)computationStage {
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

//*
- (void)imageFlow:(EASImageFlow *)imageFlow didCompleteComputationStage:(EASComputationStage)computationStage {
	switch (computationStage) {
		case EASComputationStageGenerateGrayscaleImage:
			[self displayProducedImage:[imageFlow grayscaleImage]];
			break;
		
		case EASComputationStageDetectEdges:
			[self displayProducedImage:[imageFlow detectedEdgesImage]];
			break;
		
	  default:
			break;
	}
}

- (void)displayProducedImage:(UIImage *)image {
	// Display the image.
	dispatch_sync(dispatch_get_main_queue(), ^{
		UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
		imageView.bounds = [[UIScreen mainScreen] bounds];
		[self.view addSubview:imageView];
	});
}
// */

- (void)imageFlowDidCompleteAllComputations:(EASImageFlow * __unused)imageFlow {
	// Draw the ordered edge points.
	NSArray<NSValue *> * __block points = [self.imageFlow getOrderedEdgePoints];
	dispatch_sync(dispatch_get_main_queue(), ^{
		[self.screenVC addPoints:points animated:NO]; // TODO: try out animation
	});
}

@end
