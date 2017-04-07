//
//  EASScreenView.m
//  EtchASketch
//
//  Created by Justin Loew on 10/14/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import "EASScreenView.h"

@interface EASScreenView ()

/// The points to be drawn.
@property (nonatomic, readwrite, nonnull) NSMutableArray<NSValue *> *points;

/// A layer containing a dot at each point.
@property (nonatomic, weak, nullable) CAShapeLayer *pointsLayer;
/// A layer containing the path connecting the points.
@property (nonatomic, weak, nullable) CAShapeLayer *pathLayer;

/// Draw each individual point.
- (void)drawPoints;

/// Draw a Bézier path connecting the points.
- (void)connectPoints;

/// Create a Bézier path with a line connecting each point to the next.
- (UIBezierPath *)pathConnectingPoints;

/// Create a Bézier path with a dot at the position of each point.
- (UIBezierPath *)pathDrawingPoints;

@end


@implementation EASScreenView

#pragma mark Initialization

- (instancetype)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self) {
		[self commonInit];
	}
	return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		[self commonInit];
	}
	return self;
}

- (void)commonInit {
	self.backgroundColor = [UIColor lightGrayColor];
	self.points = [NSMutableArray array];
	self.pointColor = [UIColor blackColor];
	self.lineColor = [UIColor darkGrayColor];
	self.shouldDrawPoints = NO;
	self.shouldDrawPath = YES;
}

- (void)setPoints:(NSArray<NSValue *> *)newPoints animated:(BOOL)animated {
	// Add the new points.
	_points = [newPoints mutableCopy];
	
	// Redraw.
	if (!animated) {
		[self setNeedsDisplay];
		return;
	}
	
	// Begin the animation.
	CABasicAnimation *pathAnimation = [CABasicAnimation
									   animationWithKeyPath:@"strokeEnd"];
	pathAnimation.duration = 10.0;
	pathAnimation.fromValue = @(0.0f);
	pathAnimation.toValue = @(1.0f);
	
	// Set up our points layer.
	if (self.shouldDrawPoints) {
		CAShapeLayer *pointsLayer = [CAShapeLayer layer];
		pointsLayer.path = [self pathDrawingPoints].CGPath;
		pointsLayer.strokeColor = self.pointColor.CGColor;
		pointsLayer.fillColor = nil;
		pointsLayer.lineWidth = 3.0f;
		pointsLayer.lineJoin = kCALineJoinMiter;
		if (self.pointsLayer) {
			[self.layer replaceSublayer:self.pointsLayer with:pointsLayer];
		} else {
			[self.layer addSublayer:pointsLayer];
		}
		self.pointsLayer = pointsLayer;
		[self.pointsLayer addAnimation:pathAnimation forKey:@"draw points"];
	}
	
	// Set up our path layer.
	if (self.shouldDrawPath) {
		CAShapeLayer *pathLayer = [CAShapeLayer layer];
		pathLayer.path = [[self pathConnectingPoints] CGPath];
		pathLayer.strokeColor = self.lineColor.CGColor;
		pathLayer.fillColor = nil;
		pathLayer.lineWidth = 1.0f;
		pathLayer.lineJoin = kCALineJoinMiter;
		if (self.pathLayer) {
			[self.layer replaceSublayer:self.pathLayer with:pathLayer];
		} else {
			[self.layer addSublayer:pathLayer];
		}
		self.pathLayer = pathLayer;
		[self.pathLayer addAnimation:pathAnimation forKey:@"draw path"];
	}
}

#pragma mark Drawing

- (void)drawRect:(CGRect __unused)rect {
	if (self.shouldDrawPoints) {
		[self drawPoints];
	}
	if (self.shouldDrawPath) {
		[self connectPoints];
	}
}

- (void)drawPoints {
	// Set up how we're going to draw the points.
	const CGFloat pointSize = 5.0;
	const CGFloat offset = pointSize / 2.0;
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextSetFillColorWithColor(ctx, [self.pointColor CGColor]);
	// Draw each point.
	for (NSValue *point in self.points) {
		CGPoint pt = point.CGPointValue;
		CGRect rect = CGRectMake(self.frame.origin.x + pt.x - offset,
								 self.frame.origin.y + pt.y - offset,
								 pointSize, pointSize);
		CGContextFillEllipseInRect(ctx, rect);
	}
}

- (void)connectPoints {
	UIBezierPath *pointsPath = [self pathConnectingPoints];
	[pointsPath stroke];
}

- (UIBezierPath *)pathConnectingPoints {
	// Connect the points with a bezier path.
	UIBezierPath *path = [UIBezierPath bezierPath];
	[path moveToPoint:CGPointMake(0, 0)];
	path.lineWidth = 1.0;
	path.lineJoinStyle = kCGLineJoinRound;
	
	// Add each point to the path, in order.
	for (NSValue *point in self.points) {
		[path addLineToPoint:point.CGPointValue];
	}
	
	return path;
}

- (UIBezierPath *)pathDrawingPoints {
	UIBezierPath *path = [UIBezierPath bezierPath];
	
	for (NSValue *point in self.points) {
		CGPoint cgPoint = point.CGPointValue;
		[path moveToPoint:cgPoint];
		[path addArcWithCenter:cgPoint
						radius:2.5
					startAngle:0.0
					  endAngle:2.0 * M_PI
					 clockwise:NO];
		[path closePath];
	}
	
	return path;
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

// For debugging and documentation purposes.
- (UIImage *)captureView {
	// From https://stackoverflow.com/questions/17145049/capture-uiview-and-save-as-image
	CGRect rect = [self bounds];
	UIGraphicsBeginImageContext(rect.size);
	CGContextRef context = UIGraphicsGetCurrentContext();
	[self.layer renderInContext:context];
	UIImage *img = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	return img;
}

@end
