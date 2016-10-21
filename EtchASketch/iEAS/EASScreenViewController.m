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
	
	// Display message for no points
	UILabel *msgLabel = [[UILabel alloc] initWithFrame:self.view.frame];
	msgLabel.text = @"No Points";
	msgLabel.textAlignment = NSTextAlignmentCenter;
	msgLabel.lineBreakMode = NSLineBreakByWordWrapping;
	msgLabel.numberOfLines = 0;
	msgLabel.adjustsFontSizeToFitWidth = YES;
	msgLabel.allowsDefaultTighteningForTruncation = YES;
	msgLabel.center = self.view.center;
	[self.view addSubview:msgLabel];
}

- (void)addPoints:(NSArray<NSValue *> *)points animated:(BOOL)animated {
	// TODO: this
}

@end
