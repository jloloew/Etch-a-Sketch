//
//  EASBoardViewController.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASImageFlow.h"
#import "EASScreenView.h"

NS_ASSUME_NONNULL_BEGIN
@interface EASBoardViewController : UIViewController <EASImageFlowDelegate>

/// Contains the image of the Etch-a-Sketch board and background.
@property (weak, nonatomic) IBOutlet UIImageView *boardImageView;
/// Contains the contents of the Etch-a-Sketch's screen.
@property (weak, nonatomic) IBOutlet EASScreenView *screenView;
/// The component that performs the actual computation.
@property (strong, nonatomic) EASImageFlow *imageFlow;

@end
NS_ASSUME_NONNULL_END
