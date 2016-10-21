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

@end
NS_ASSUME_NONNULL_END
