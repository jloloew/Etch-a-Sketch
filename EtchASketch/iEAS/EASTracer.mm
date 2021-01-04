//
//  EASTracer.m
//  iEAS
//
//  Created by Justin Loew on 1/4/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#include <stdexcept>

#include <Tracer/Tracer.hpp>

#import "EASTracer.h"
#import "EASTracer+CPP.hh"

using tracer::Tracer;


#pragma mark TracerDelegateBridge

__attribute__((pure))
inline static int
generateMask(ietchasketch::TracerDelegateBridge::DelegateMethod methIdx)
{
    return 0x1 << methIdx;
}

inline bool
ietchasketch::TracerDelegateBridge::shouldForward(DelegateMethod methIdx) const {
    int mask = generateMask(methIdx);
    return (callbackForwardingMask & mask) != 0;
}

ietchasketch::TracerDelegateBridge::TracerDelegateBridge(_Nonnull id<EASTracerDelegate> objcDelegate, EASTracer * _Nonnull objcTracer)
: objcDelegate(objcDelegate), objcTracer(objcTracer)
{
    // Store which Objective-C delegate methods are implemented ahead of time. This improves performance.
    int cbForwardingMask = ~0x0;
    for (int i = 0; i < static_cast<int>(DELEGATE_METHODS_COUNT); i++) {
        DelegateMethod methIdx = static_cast<DelegateMethod>(i);
        SEL delegateSelector;
        switch (methIdx) {
            case willBegin:
                delegateSelector = @selector(tracerWillBeginTracing:);
                break;
            case didBegin:
                delegateSelector = @selector(tracerDidBeginTracing:);
                break;
            case willTrace:
                delegateSelector = @selector(tracer:willTracePoint:);
                break;
            case didTrace:
                delegateSelector = @selector(tracer:didTracePoint:);
                break;
            case willEnd:
                delegateSelector = @selector(tracerWillEndTracing:);
                break;
            case didEnd:
                delegateSelector = @selector(tracerDidEndTracing:);
                break;
            case didEncounterError:
                delegateSelector = @selector(tracer:didEncounterError:);
                break;
            default:
                // The DelegateMethod enum definition is missing a value.
                throw std::out_of_range("Unhandled delegate method index. Update the implementation.");
        }
        if (![objcDelegate respondsToSelector:delegateSelector]) {
            // Mask off this callback.
            int mask = ~generateMask(methIdx);
            cbForwardingMask &= mask;
        }
    }
    callbackForwardingMask = cbForwardingMask;
//    this->objcDelegate = objcDelegate;
//    this->objcTracer = objcTracer;
}

void
ietchasketch::TracerDelegateBridge::tracerWillBeginTracing(Tracer * _Nonnull tracer __unused)
{
    DelegateMethod methIdx = willBegin;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracerWillBeginTracing:objcTracer];
}

void
ietchasketch::TracerDelegateBridge::tracerDidBeginTracing(Tracer * _Nonnull tracer __unused)
{
    DelegateMethod methIdx = didBegin;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracerDidBeginTracing:objcTracer];
}


void
ietchasketch::TracerDelegateBridge::tracerWillTracePoint(Tracer * _Nonnull tracer __unused, tracer::Point pt)
{
    DelegateMethod methIdx = willTrace;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracer:objcTracer willTracePoint:CGPointMake(pt.x, pt.y)];
}

void
ietchasketch::TracerDelegateBridge::tracerDidTracePoint(Tracer * _Nonnull tracer __unused, tracer::Point pt)
{
    DelegateMethod methIdx = didTrace;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracer:objcTracer didTracePoint:CGPointMake(pt.x, pt.y)];
}


void
ietchasketch::TracerDelegateBridge::tracerWillEndTracing(Tracer * _Nonnull tracer __unused)
{
    DelegateMethod methIdx = willEnd;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracerWillEndTracing:objcTracer];
}

void
ietchasketch::TracerDelegateBridge::tracerDidEndTracing(Tracer * _Nonnull tracer __unused)
{
    DelegateMethod methIdx = didEnd;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    [objcDelegate tracerDidEndTracing:objcTracer];
}


void
ietchasketch::TracerDelegateBridge::tracerDidEncounterError(Tracer * _Nonnull tracer __unused, std::exception &ex)
{
    DelegateMethod methIdx = didEncounterError;
    if (!shouldForward(methIdx)) {
        return;
    }
    // Forward this notification to the objc delegate.
    // FIXME: Get our bundle identifier programmatically.
    NSString *bundleId = @"com.justinloew.iEAS";
    NSString *errorDomain = [bundleId stringByAppendingString:@".TracerError"];
    NSDictionary *userInfo = @{ NSLocalizedDescriptionKey : [NSString stringWithUTF8String:ex.what()] };
    NSError *error = [NSError errorWithDomain:errorDomain code:1 userInfo:userInfo];
    [objcDelegate tracer:objcTracer didEncounterError:error];
}


NS_ASSUME_NONNULL_BEGIN
@interface EASTracer ()

@property (nonatomic, readwrite) tracer::Tracer *tracer;
@property (nonatomic, nullable) ietchasketch::TracerDataSourceBridge *dataSourceBridge;
@property (nonatomic, nullable) ietchasketch::TracerDelegateBridge *delegateBridge;

/// A serial queue on which work is synchronously run.
@property (nonatomic, readonly) dispatch_queue_t processingQueue;

/**
 * A serial queue on which delegate callbacks are asynchronously run.
 * This is just the global dispatch queue, at least for now.
 */
@property (nonatomic, readonly) dispatch_queue_t delegateQueue;

@end
NS_ASSUME_NONNULL_END


@implementation EASTracer

- (instancetype)initWithDataSource:(id<EASTracerDataSource>)dataSource {
    self = [super init];
    if (self) {
        self.tracer = nullptr;
        self.dataSourceBridge = nullptr;
        self.delegateBridge = nullptr;
        self.dataSource = dataSource;
        self.delegate = nil;
    }
    return self;
}

- (void)dealloc {
    if (self.dataSourceBridge) {
        delete self.dataSourceBridge;
        self.dataSourceBridge = nullptr;
        self.tracer->setDataSource(nullptr);
    }
    if (self.delegateBridge) {
        delete self.delegateBridge;
        self.delegateBridge = nullptr;
        self.tracer->setDelegate(nullptr);
    }
    delete self.tracer;
}

- (void)setDataSource:(id<EASTracerDataSource>)dataSource {
    if (self.dataSourceBridge) {
        delete self.dataSourceBridge;
        self.dataSourceBridge = nullptr;
    }

    ietchasketch::TracerDataSourceBridge *dsb;
    if (dataSource) {
        dsb = new ietchasketch::TracerDataSourceBridge(dataSource, self);
        self.dataSourceBridge = dsb;
    } else {
        dsb = nullptr;
    }
    self.tracer->setDataSource(dsb);
}

- (void)setDelegate:(id<EASTracerDelegate>)delegate {
    if (self.delegateBridge) {
        delete self.delegateBridge;
        self.delegateBridge = nullptr;
    }

    ietchasketch::TracerDelegateBridge *db;
    if (delegate) {
        db = new ietchasketch::TracerDelegateBridge(delegate, self);
        self.delegateBridge = db;
    } else {
        db = nullptr;
    }
    self.tracer->setDelegate(db);
}

@end
