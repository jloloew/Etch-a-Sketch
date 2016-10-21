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

- (instancetype)initWithImage:(UIImage *)image {
	self = [self initWithWidth:image.size.width height:image.size.height];
	if (self != nil) {
		CGImageRef img = [image CGImage];
		// Draw the image into a buffer.
		NSUInteger width = CGImageGetWidth(img);
		NSUInteger height = CGImageGetHeight(img);
		NSUInteger bytesPerPixel = 4;
		NSUInteger bytesPerRow = bytesPerPixel * width;
		NSUInteger bitsPerComponent = 8;
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		UInt8 *rawData = (UInt8 *)calloc(width * height * bytesPerPixel, sizeof(UInt8));
		if (nullptr == rawData) {
			return nil;
		}
		CGContextRef ctx = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace/*/ nullptr*/, kCGImageAlphaNoneSkipFirst);
		CGColorSpaceRelease(colorSpace);
		CGContextDrawImage(ctx, CGRectMake(0, 0, width, height), img);
		CGContextRelease(ctx);
		
		// Create the image
		_image = new Image(width, height,
						   reinterpret_cast<uint32_t *>(rawData));
		free(rawData);
		rawData = nullptr;
		if (nullptr == _image) {
			return nil;
		}
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
