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

@end

@implementation EASBoardViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	
	// Set up screen VC.
	self.screenVC = [[EASScreenViewController alloc] initWithNibName:@"EASScreenViewController" bundle:nil];
	self.screenVC.view.frame = self.screenViewPlaceholder.frame;
	self.screenViewPlaceholder = nil;
}

@end
