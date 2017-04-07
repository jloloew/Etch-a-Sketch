//
//  EASScreenView.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;

NS_ASSUME_NONNULL_BEGIN

/// Represents the contents of an Etch-a-Sketch's drawable area.
IB_DESIGNABLE
@interface EASScreenView : UIView

/// The points to display.
@property (nonatomic, readonly) NSArray<NSValue *> *points;

/// The color in which the points are displayed.
@property (nonatomic) IBInspectable UIColor *pointColor;

/// The color in which the lines connecting the points are displayed.
@property (nonatomic) IBInspectable UIColor *lineColor;

/// Whether points should be displayed at all. Default is @c NO.
@property (nonatomic) IBInspectable BOOL shouldDrawPoints;

/**
 * Whether a path between the points should be displayed at all. Default is
 * @c YES.
 */
@property (nonatomic) IBInspectable BOOL shouldDrawPath;

/**
 * Change the points displayed with an optional animation.
 * 
 * @param newPoints The new points to be drawn, as @c NSValues containing
 * @c CGPoints.
 * @param animated Whether to animate the drawing of the points.
 */
- (void)setPoints:(NSArray<NSValue *> *)newPoints animated:(BOOL)animated;

@end
NS_ASSUME_NONNULL_END
