//
//  EASScreenView.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;

NS_ASSUME_NONNULL_BEGIN
IB_DESIGNABLE
@interface EASScreenView : UIView

/// The points to display.
@property (nonatomic, readonly, nullable) NSArray<NSValue *> *points;

/// The color in which the points are displayed.
@property (nonatomic) IBInspectable UIColor *pointColor;

/// The color in which the lines connecting the points are displayed.
@property (nonatomic) IBInspectable UIColor *lineColor;

- (void)addPoints:(NSArray<NSValue *> *)newPoints animated:(BOOL)animated;

@end
NS_ASSUME_NONNULL_END
