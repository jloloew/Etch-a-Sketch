//
//  EASImage+CPP.hh
//  EtchASketch
//
//  Created by Justin Loew on 4/13/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef EASImage_CPP_hh
#define EASImage_CPP_hh

#import "EASImage.h"
#import "EtchASketch.hpp"

NS_ASSUME_NONNULL_BEGIN
@interface EASImage (CPP)

/// The wrapped C++ backing instance.
@property (nonatomic, readonly, nonnull) const etchasketch::Image *image;

/**
 * @Note: Uses the same C++ image instance. This init is meant for short
 * lifespan EASImages where the EASImage will be freed shortly after use.
 */
- (instancetype)initWithCPPImage:(const etchasketch::Image *)image;

@end
NS_ASSUME_NONNULL_END

#endif /* EASImage_CPP_hh */
