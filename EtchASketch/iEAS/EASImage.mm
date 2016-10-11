//
//  EASImage.m
//  EtchASketch
//
//  Created by Justin Loew on 10/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASImage.h"
#import "EtchASketch.hpp"

@interface EASImage ()

@property (nonatomic, readonly) etchasketch::Image *image;

@end


@implementation EASImage

- (instancetype)initWithWidth:(NSUInteger)width
					   height:(NSUInteger)height
{
	self = [super init];
	if (self != nil) {
		_image = new etchasketch::Image(width, height);
	}
	return self;
}

- (void)dealloc {
	delete self.image;
	_image = nil;
}

#pragma mark - Getters

- (NSUInteger)width {
	return (NSUInteger) self.image->getWidth();
}

- (NSUInteger)height {
	return (NSUInteger) self.image->getHeight();
}

@end
