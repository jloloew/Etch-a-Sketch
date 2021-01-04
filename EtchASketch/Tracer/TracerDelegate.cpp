//
//  TracerDelegate.cpp
//  Tracer
//
//  Created by Justin Loew on 1/4/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#include "TracerDelegate.hpp"

void
tracer::TracerDelegate::tracerWillBeginTracing(tracer::Tracer *tracer)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerDidBeginTracing(tracer::Tracer *tracer)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerWillTracePoint(tracer::Tracer *tracer, tracer::Point pt)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerDidTracePoint(tracer::Tracer *tracer, tracer::Point pt)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerWillEndTracing(tracer::Tracer *tracer)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerDidEndTracing(tracer::Tracer *tracer)
{
    // Do nothing.
}

void
tracer::TracerDelegate::tracerDidEncounterError(tracer::Tracer *tracer, std::exception &ex)
{
    // Do nothing.
}
