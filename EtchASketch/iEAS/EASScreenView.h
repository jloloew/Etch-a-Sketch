//
//  EASScreenView.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

@import UIKit;

IB_DESIGNABLE
@interface EASScreenView : UIView

/// The points to display.
@property (nonatomic) NSArray<NSValue *> *points;

/// The color in which the points are displayed.
@property (nonatomic) IBInspectable UIColor *pointColor;

@end
