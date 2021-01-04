//
//  EASTracer+CPP.hh
//  iEAS
//
//  Created by Justin Loew on 1/4/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#ifndef EASTracer_CPP_h
#define EASTracer_CPP_h

#import "EASTracer.h"
#import "Tracer/Tracer.hpp"


namespace ietchasketch {

    class TracerDataSourceBridge : public tracer::TracerDataSource {
    public:
        TracerDataSourceBridge(_Nonnull id<EASTracerDataSource> objcDataSource, EASTracer * _Nonnull objcTracer);

        virtual tracer::TracerDataSource::Iterator begin();
        virtual tracer::TracerDataSource::Iterator end();
        virtual tracer::Point tracerStartingPoint(tracer::Tracer * _Nonnull tracer);

    private:
        _Nonnull id<EASTracerDataSource> objcDataSource;
        EASTracer * _Nonnull objcTracer;
    };


    class TracerDelegateBridge : public tracer::TracerDelegate {
    public:
        TracerDelegateBridge(_Nonnull id<EASTracerDelegate> objcDelegate, EASTracer * _Nonnull objcTracer);

        virtual void tracerWillBeginTracing(tracer::Tracer * _Nonnull tracer);
        virtual void tracerDidBeginTracing(tracer::Tracer * _Nonnull tracer);

        virtual void tracerWillTracePoint(tracer::Tracer * _Nonnull tracer, tracer::Point pt);
        virtual void tracerDidTracePoint(tracer::Tracer * _Nonnull tracer, tracer::Point pt);

        virtual void tracerWillEndTracing(tracer::Tracer * _Nonnull tracer);
        virtual void tracerDidEndTracing(tracer::Tracer * _Nonnull tracer);

        virtual void tracerDidEncounterError(tracer::Tracer * _Nonnull tracer, std::exception &ex);

        typedef enum {
            willBegin = 0,
            didBegin,
            willTrace,
            didTrace,
            willEnd,
            didEnd,
            didEncounterError,
            DELEGATE_METHODS_COUNT
        } DelegateMethod;

    private:
        _Nonnull id<EASTracerDelegate> objcDelegate;
        EASTracer * _Nonnull objcTracer;
        int callbackForwardingMask;

        inline bool shouldForward(DelegateMethod methIdx) const;
    };


    class TracerBridge : public tracer::TracerDefaults {
    public:
        // FIXME: Write this class.

















    };

}

#endif /* EASTracer_CPP_h */
