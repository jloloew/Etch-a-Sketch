//
//  TracerDataSource.hpp
//  EtchASketch
//
//  Created by Justin Loew on 1/3/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#ifndef TracerDataSource_h
#define TracerDataSource_h

#include <iterator>

#include "TracerPoint.hpp"

namespace tracer {

    class Tracer;

    /**
     * Supplies a Tracer with a sequence of points to trace.
     */
    class TracerDataSource {
    public:
        /**
         * An iterator over a sequence of points to be traced.
         */
        typedef std::iterator<std::input_iterator_tag, Point> Iterator;

        virtual ~TracerDataSource();

        /**
         * The first point to move to.
         *
         * This need not be the same as the tracer's starting point.
         */
        virtual Iterator begin() = 0;

        /**
         * The point past the final point to move to.
         */
        virtual Iterator end() = 0;

        /**
         * The location of the tracer's nib, before we've drawn any points.
         *
         * The default implementation assumes the nib begins at (0, 0).
         */
        virtual Point tracerStartingPoint(Tracer *tracer);
    };

}

#endif /* TracerDataSource_h */
