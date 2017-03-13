//
//  EASImage.h
//  EtchASketch
//
//  Created by Justin Loew on 10/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN
@interface EASImage : NSObject

- (instancetype)initWithWidth:(NSUInteger)width
					   height:(NSUInteger)height;

- (instancetype)initWithImage:(UIImage *)image;

/// Check whether the image is valid.
- (BOOL)isValid;

/// The width of the image, in pixels.
- (NSUInteger)width;

/// The height of the image, in pixels.
- (NSUInteger)height;

/// Return a @c UIImage of the @c EASImage, performing conversion if necessary.
- (nullable UIImage *)UIImage;

/**
 * Read in a @c .etch file and attempt to convert it to an @c EASImage. For
 * debugging purposes.
 */
+ (EASImage *)imageFromEtchFileAtPath:(NSString *)path;

@end
NS_ASSUME_NONNULL_END
