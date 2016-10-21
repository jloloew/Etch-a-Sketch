//
//  AppDelegate.h
//  iEAS
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASBoardViewController.h"

NS_ASSUME_NONNULL_BEGIN
@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) EASBoardViewController *boardVC;

@end
NS_ASSUME_NONNULL_END
