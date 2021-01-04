//
//  EASTracer.h
//  iEAS
//
//  Created by Justin Loew on 1/4/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class EASTracer;


@protocol EASTracerDataSource <NSObject>
@optional
- (CGPoint)tracerStartingPoint:(EASTracer *)tracer;
@end


@protocol EASTracerDelegate <NSObject>

@optional
- (void)tracerWillBeginTracing:(EASTracer *)tracer;
@optional
- (void)tracerDidBeginTracing:(EASTracer *)tracer;

@optional
- (void)tracer:(EASTracer *)tracer willTracePoint:(CGPoint)point;
@optional
- (void)tracer:(EASTracer *)tracer didTracePoint:(CGPoint)point;

@optional
- (void)tracerWillEndTracing:(EASTracer *)tracer;
@optional
- (void)tracerDidEndTracing:(EASTracer *)tracer;

@optional
- (void)tracer:(EASTracer *)tracer didEncounterError:(NSError *)error;

@end


@interface EASTracer : NSObject

@property (nonatomic) id<EASTracerDataSource> dataSource;
@property (nonatomic, nullable) id<EASTracerDelegate> delegate;

- (instancetype)initWithDataSource:(id<EASTracerDataSource>)dataSource NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
