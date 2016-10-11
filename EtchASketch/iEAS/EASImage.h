//
//  EASImage.h
//  EtchASketch
//
//  Created by Justin Loew on 10/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface EASImage : NSObject

@property (nonatomic, readonly) NSUInteger width;
@property (nonatomic, readonly) NSUInteger height;

- (instancetype)initWithWidth:(NSUInteger)width
					   height:(NSUInteger)height;

@end
