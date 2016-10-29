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
	
	// Begin computation.
	[self doComputationSequence];
}

- (void)doComputationSequence {
	/*
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
	});
}

#pragma mark EASImageFlowDelegate

- (void)imageFlow:(EASImageFlow * __unused)imageFlow willBeginComputationStage:(EASComputationStage)computationStage {
	// Set the status label for the current stage of computation.
	dispatch_sync(dispatch_get_main_queue(), ^{
		switch (computationStage) {
			case EASComputationStageNone:
				self.statusLabel.text = @"Loading…";
				break;
			case EASComputationStageGenerateGrayscaleImage:
				self.statusLabel.text = @"Generating grayscale image…";
				break;
			case EASComputationStageDetectEdges:
				self.statusLabel.text = @"Detecting edges…";
				break;
			case EASComputationStageGenerateEdgePoints:
				self.statusLabel.text = @"Generating edge points…";
				break;
			case EASComputationStageOrderEdgePointsForDrawing:
				self.statusLabel.text = @"Ordering edge points for drawing…";
				break;
			case EASComputationStageFinished:
				self.statusLabel.text = @"Done!";
				break;
		}
	});
}

/*
- (void)imageFlow:(EASImageFlow *)imageFlow didCompleteComputationStage:(EASComputationStage)computationStage {
	if (computationStage == EASComputationStageGenerateGrayscaleImage) {
		// Display the grayscale image.
		dispatch_sync(dispatch_get_main_queue(), ^{
			UIImage *grayscaleImage = [imageFlow grayscaleImage];
			UIImageView *grayView = [[UIImageView alloc] initWithImage:grayscaleImage];
			grayView.bounds = [[UIScreen mainScreen] bounds];
			[self.view addSubview:grayView];
		});
	}
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
