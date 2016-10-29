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

@property (nonatomic, readonly) const Image *image;

@property (nonatomic, nullable) UIImage *backingUIImage;

@property (nonatomic) BOOL shouldFreeCPPImage;

/**
 * @Note: Uses the same C++ image instance. This init is meant for short 
 * lifespan EASImages where the EASImage will be freed shortly after use.
 */
- (instancetype)initWithCPPImage:(const Image *)image;

- (CGImageRef)newCGImageRepresentation;

@end


@implementation EASImage

- (instancetype)initWithWidth:(NSUInteger)width
					   height:(NSUInteger)height
{
	self = [super init];
	if (self) {
		self.shouldFreeCPPImage = YES;
		self.backingUIImage = nil;
		_image = new Image(width, height);
	}
	return self;
}

- (instancetype)initWithImage:(UIImage *)image {
	self = [self initWithWidth:(NSUInteger)image.size.width height:(NSUInteger)image.size.height];
	if (self) {
		self.backingUIImage = image;
		CGImageRef img = [self.backingUIImage CGImage];
		
		// Draw the image into a buffer.
		NSUInteger width = CGImageGetWidth(img);
		NSUInteger height = CGImageGetHeight(img);
		NSUInteger bytesPerPixel = 4;
		NSUInteger bytesPerRow = bytesPerPixel * width;
		NSUInteger bitsPerComponent = 8;
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		size_t rawDataSize = width * height * bytesPerPixel;
		UInt8 *rawData = (UInt8 *)calloc(rawDataSize, sizeof(UInt8));
		if (nullptr == rawData) {
			CGColorSpaceRelease(colorSpace);
			return nil;
		}
		CGContextRef ctx = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace/*/ nullptr*/, kCGImageAlphaNoneSkipFirst);
		CGColorSpaceRelease(colorSpace);
		CGContextDrawImage(ctx, CGRectMake(0, 0, width, height), img);
		CGContextRelease(ctx);
		
		// Copy the image into the C++ image's buffer.
		
		memcpy(self.image->getData(), rawData, rawDataSize);
//		// Create the image
//		_image = new Image(width, height,
//						   reinterpret_cast<Image::Pixel *>(rawData));
		free(rawData);
		rawData = nullptr;
//		if (nullptr == _image) {
//			return nil;
//		}
	}
	return self;
}

- (instancetype)initWithCPPImage:(const etchasketch::Image *)image {
	self = [super init];
	if (self) {
		if (!image) {
			return nil;
		}
		/*
		 * This initializer is meant for creating a temporary EASImage to wrap
		 * around an existing C++ Image. We don't make a copy of it, but we
		 * won't free it either.
		 */
		self.shouldFreeCPPImage = NO;
		self.backingUIImage = nil;
		_image = image;
	}
	return self;
}

- (void)dealloc {
	if (self.shouldFreeCPPImage) {
		delete self.image;
		_image = nil;
	}
}

- (BOOL)isValid {
	return self.image->isValid();
}

- (CGImageRef)newCGImageRepresentation {
	size_t width = [self width];
	size_t height = [self height];
	size_t bitsPerComponent = 8;
	size_t bytesPerPixel = 4;
	size_t bitsPerPixel = bytesPerPixel * 8;
	size_t bytesPerRow = width * bytesPerPixel;
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGBitmapInfo bitmapInfo = kCGBitmapByteOrder32Host | kCGImageAlphaNoneSkipFirst;
	// Create the data provider.
	const void *data = (const void *)self.image->getData();
	size_t size = self.image->getPixelCount() * sizeof(etchasketch::Image::Pixel);
	CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, data, size, nullptr);
	// Create the image.
	CGImageRef image = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpace, bitmapInfo, provider, nullptr, false, kCGRenderingIntentPerceptual);
	// Free up the used objects.
	CGColorSpaceRelease(colorSpace);
	CGDataProviderRelease(provider);
	
	return image;
}

- (UIImage *)UIImage {
	// TODO: Add synchronization for this
	if (!self.backingUIImage) {
		// Generate the backing CGImage from the C++ Image.
		CGImageRef backingCGImage = [self newCGImageRepresentation];
		self.backingUIImage = [UIImage imageWithCGImage:backingCGImage];
		CGImageRelease(backingCGImage);
	}
	
	return self.backingUIImage;
}

#pragma mark - Getters

- (NSUInteger)width {
	return static_cast<NSUInteger>(self.image->getWidth());
}

- (NSUInteger)height {
	return static_cast<NSUInteger>(self.image->getHeight());
}

@end
