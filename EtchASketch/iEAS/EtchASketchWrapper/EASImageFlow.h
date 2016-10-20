//
//  EASImageFlow.h
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EASImage.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * The top-level object that takes an image and walks it through each stage of
 * processing.
 */
@interface EASImageFlow : NSObject

/// Create a new flow with a starting image.
- (instancetype)initWithColorImage:(EASImage *)colorImage;

/// Detect edges in the starting image.
- (void)detectEdges;

/// Get a set of all points on an edge in the edge detected image.
- (void)generateEdgePoints;

/// Put the edge points in the best order for drawing.
- (void)orderEdgePointsForDrawing;

@end
NS_ASSUME_NONNULL_END
