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
	
	// Set up screen VC.
	self.screenVC = [[EASScreenViewController alloc] initWithNibName:@"EASScreenViewController" bundle:nil];
	self.screenVC.view.frame = self.screenViewPlaceholder.frame;
	self.screenViewPlaceholder = nil;
	
	// Begin computation.
	[self doComputationSequence];
}

- (void)doComputationSequence {
	self.statusLabel.text = @"Detecting edges…";
	[self.imageFlow detectEdges];
	
	self.statusLabel.text = @"Generating edge points…";
	[self.imageFlow generateEdgePoints];
	
	self.statusLabel.text = @"Ordering edge points for drawing…";
	[self.imageFlow orderEdgePointsForDrawing];
	
	self.statusLabel.text = @"Drawing…";
	NSArray<NSValue *> *points = [self.imageFlow getOrderedEdgePoints];
	[self.screenVC addPoints:points animated:NO]; // TODO: try out animation
}

@end
