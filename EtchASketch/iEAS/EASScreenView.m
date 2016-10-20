//
//  EASScreenView.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASScreenView.h"

@interface EASScreenView ()

@property (nonatomic, readwrite, nullable) NSMutableArray<NSValue *> *points;

/// Draw all the points in the points array.
- (void)drawAllPoints;

@end


@implementation EASScreenView

#pragma mark Initialization

- (instancetype)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self commonInit];
	}
	return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self != nil) {
		[self commonInit];
	}
	return self;
}

- (void)commonInit {
	self.points = nil;
	self.pointColor = [UIColor redColor];//self.tintColor;
	
	self.backgroundColor = [UIColor whiteColor];
}

- (void)addPoints:(NSArray<NSValue *> *)newPoints {
	[_points addObjectsFromArray:newPoints];
	[self setNeedsDisplay];
	// TODO: Set up a way to redraw just the newly added points.
}

#pragma mark Drawing

- (void)drawRect:(CGRect)rect {
	[self drawAllPoints];
}

- (void)drawAllPoints {
	// Set up how we're going to draw the points.
	const CGFloat pointSize = 5.0;
	const CGFloat offset = pointSize / 2.0;
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetFillColor(ctx, CGColorGetComponents(self.pointColor.CGColor));
	// Draw each point.
	for (NSValue *point in self.points) {
		CGPoint pt = point.CGPointValue;
		CGRect rect = CGRectMake(self.frame.origin.x + pt.x - offset, self.frame.origin.y + pt.y - offset,
								 pointSize, pointSize);
		CGContextFillEllipseInRect(ctx, rect);
	}
}

- (void)prepareForInterfaceBuilder {
	// Generate fake points to draw, arranged in a square.
	NSMutableArray<NSValue *> *points = [NSMutableArray new];
	const NSUInteger sideLength = 100;
	const CGPoint startPoint = CGPointMake(10, 10);
	CGPoint currPoint = startPoint;
	// Top edge
	for (NSUInteger i = 0; i < sideLength; i++, currPoint.x += 1) {
		[points addObject:[NSValue valueWithCGPoint:currPoint]];
	}
	// Right edge
	for (NSUInteger i = 0; i < sideLength; i++, currPoint.y += 1) {
		[points addObject:[NSValue valueWithCGPoint:currPoint]];
	}
	// Bottom edge
	for (NSUInteger i = 0; i < sideLength; i++, currPoint.x -= 1) {
		[points addObject:[NSValue valueWithCGPoint:currPoint]];
	}
	// Left edge
	for (NSUInteger i = 0; i < sideLength; i++, currPoint.y -= 1) {
		[points addObject:[NSValue valueWithCGPoint:currPoint]];
	}
	
	self.points = points;
}

#pragma mark Setters

- (void)setPoints:(NSMutableArray<NSValue *> *)points {
	_points = points;
	[self setNeedsDisplay];
}

- (void)setPointColor:(UIColor *)pointColor {
	_pointColor = pointColor;
	[self setNeedsDisplay];
}

@end
