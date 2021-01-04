//
//  TracerDelegate.hpp
//  EtchASketch
//
//  Created by Justin Loew on 1/4/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#ifndef TracerDelegate_h
#define TracerDelegate_h

#include <exception>

#include "TracerPoint.hpp"

namespace tracer {

    class Tracer;

    /**
     * Handles lifecycle events from a Tracer.
     */
    class TracerDelegate {
    public:
        virtual ~TracerDelegate();

        virtual void tracerWillBeginTracing(Tracer *tracer);
        virtual void tracerDidBeginTracing(Tracer *tracer);

        virtual void tracerWillTracePoint(Tracer *tracer, Point pt);
        virtual void tracerDidTracePoint(Tracer *tracer, Point pt);

        virtual void tracerWillEndTracing(Tracer *tracer);
        virtual void tracerDidEndTracing(Tracer *tracer);

        virtual void tracerDidEncounterError(Tracer *tracer, std::exception &ex);
    };

}

#endif /* TracerDelegate_h */
