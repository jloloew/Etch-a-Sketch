//
//  EASScreenViewController.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;
#import "EASScreenView.h"

NS_ASSUME_NONNULL_BEGIN
@interface EASScreenViewController : UIViewController

@property (weak, nonatomic) IBOutlet EASScreenView *screen;

/**
 * Add new points to the path drawn on the screen.
 * 
 * @param points The points to be added.
 * @param animated Whether the points should be drawn one by one or just appear.
 */
- (void)addPoints:(NSArray<NSValue *> *)points animated:(BOOL)animated;

@end
NS_ASSUME_NONNULL_END
