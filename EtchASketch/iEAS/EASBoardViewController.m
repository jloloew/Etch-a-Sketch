//
//  EASBoardViewController.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASBoardViewController.h"

@interface EASBoardViewController ()

@property (weak, nonatomic) IBOutlet EASScreenView *screenViewPlaceholder;
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
	self.screenVC.view.frame = self.screenViewPlaceholder.frame;
	self.screenViewPlaceholder = nil;
	
	// Begin computation.
	[self doComputationSequence];
}

- (void)doComputationSequence {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
		[self.imageFlow detectEdges];
		
		[self.imageFlow generateEdgePoints];
		
		[self.imageFlow orderEdgePointsForDrawing];
	});
}

#pragma mark EASImageFlowDelegate

- (void)imageFlow:(EASImageFlow *)imageFlow willBeginComputationStage:(EASComputationStage)computationStage {
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

- (void)imageFlowDidCompleteAllComputations:(EASImageFlow *)imageFlow {
	// Draw the ordered edge points.
	NSArray<NSValue *> *points = [self.imageFlow getOrderedEdgePoints];
	dispatch_sync(dispatch_get_main_queue(), ^{
		[self.screenVC addPoints:points animated:NO]; // TODO: try out animation
	});
}

@end
