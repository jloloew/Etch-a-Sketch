//
//  EASBoardViewController.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASScreenViewController.h"

NS_ASSUME_NONNULL_BEGIN
@interface EASBoardViewController : UIViewController

@property (weak, nonatomic) IBOutlet UIImageView *boardImageView;
@property (strong, nonatomic) EASScreenViewController *screenVC;

@end
NS_ASSUME_NONNULL_END
