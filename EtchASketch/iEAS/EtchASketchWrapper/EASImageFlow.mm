//
//  EASImageFlow.mm
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImageFlow.h"
#import "EtchASketch.hpp"

using etchasketch::Image;
using etchasketch::ImageFlow;

@interface EASImageFlow ()

@property (nonatomic, readonly) ImageFlow *imageFlow;

@end


@interface EASImage ()

@property (nonatomic, readonly) Image *image;

@end


@implementation EASImageFlow

- (instancetype)initWithColorImage:(EASImage *)colorImage {
	self = [super init];
	if (self != nil) {
		_imageFlow = new ImageFlow(*colorImage.image);
	}
	return self;
}

- (void)dealloc {
	delete self.imageFlow;
	_imageFlow = nullptr;
}

- (void)detectEdges {
	self.imageFlow->detectEdges();
}

- (void)generateEdgePoints {
	self.imageFlow->generateEdgePoints();
}

- (void)orderEdgePointsForDrawing {
	self.imageFlow->orderEdgePointsForDrawing();
}

@end
