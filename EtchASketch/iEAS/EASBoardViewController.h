//
//  EASBoardViewController.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASScreenViewController.h"
#import "EASImageFlow.h"

NS_ASSUME_NONNULL_BEGIN
@interface EASBoardViewController : UIViewController <EASImageFlowDelegate>

@property (weak, nonatomic) IBOutlet UIImageView *boardImageView;
@property (strong, nonatomic) EASScreenViewController *screenVC;
@property (strong, nonatomic) EASImageFlow *imageFlow;

@end
NS_ASSUME_NONNULL_END
