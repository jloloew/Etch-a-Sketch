//
//  EASImage.mm
//  EtchASketch
//
//  Created by Justin Loew on 10/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImage.h"
#import "EtchASketch.hpp"

using etchasketch::Image;

@interface EASImage ()

@property (nonatomic, readonly) Image *image;

@end


@implementation EASImage

- (instancetype)initWithWidth:(NSUInteger)width
					   height:(NSUInteger)height
{
	self = [super init];
	if (self != nil) {
		_image = new Image(width, height);
	}
	return self;
}

- (void)dealloc {
	delete self.image;
	_image = nil;
}

- (BOOL)isValid {
	return self.image->isValid();
}

#pragma mark - Getters

- (NSUInteger)width {
	return static_cast<NSUInteger>(self.image->getWidth());
}

- (NSUInteger)height {
	return static_cast<NSUInteger>(self.image->getHeight());
}

@end
