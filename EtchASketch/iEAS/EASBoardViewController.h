//
//  EASBoardViewController.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASScreenViewController.h"
#import "EASImageFlowManager.h"

NS_ASSUME_NONNULL_BEGIN
@interface EASBoardViewController : UIViewController <EASImageFlowManagerDelegate>

@property (weak, nonatomic) IBOutlet UIImageView *boardImageView;
@property (strong, nonatomic) EASScreenViewController *screenVC;
@property (strong, nonatomic) EASImageFlowManager *imageFlow;

@end
NS_ASSUME_NONNULL_END
