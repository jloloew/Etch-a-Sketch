//
//  EASScreenViewController.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASScreenViewController.h"

@interface EASScreenViewController ()

@end

@implementation EASScreenViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.view.backgroundColor = [UIColor clearColor];
	
//	// Display message for no points
//	UILabel *msgLabel = [[UILabel alloc] initWithFrame:self.view.frame];
//	msgLabel.backgroundColor = [UIColor clearColor];
//	msgLabel.textAlignment = NSTextAlignmentCenter;
//	msgLabel.lineBreakMode = NSLineBreakByWordWrapping;
//	msgLabel.numberOfLines = 0;
//	msgLabel.adjustsFontSizeToFitWidth = YES;
//	msgLabel.allowsDefaultTighteningForTruncation = YES;
//	msgLabel.center = self.view.center;
//	msgLabel.text = @"No Points";
//	[self.view addSubview:msgLabel];
}

- (void)addPoints:(NSArray<NSValue *> *)points animated:(BOOL __unused)animated {
	if (!animated) {
		// Just add the points, nothing fancy.
		[self.screen addPoints:points];
		return;
	}
	
	// Animate adding the points.
}

@end
