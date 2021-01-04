//
//  TracerProtocol.hpp
//  Tracer
//
//  Created by Justin Loew on 1/3/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#ifndef TracerProtocol_hpp
#define TracerProtocol_hpp

#include "TracerDataSource.hpp"
#include "TracerDelegate.hpp"

namespace tracer {

    /**
     * Traces out a sequence of points on a given medium.
     *
     * This is an abstract base class. The medium in which points are traced is determined by a concrete subclass.
     */
    class Tracer {
    public:
        virtual ~Tracer();

        virtual TracerDataSource * dataSource() const = 0;
        virtual void setDataSource(TracerDataSource *dataSource) = 0;

        virtual TracerDelegate * delegate() const = 0;
        virtual void setDelegate(TracerDelegate *delegate) = 0;
    };

    /**
     * A base Tracer class with various default implementations.
     *
     * This class exists as a convenience. It is intended to be used as a starting point for concrete Tracer subclasses.
     */
    class TracerDefaults : public Tracer {
    public:
        TracerDefaults();
        virtual ~TracerDefaults();

        virtual TracerDataSource * dataSource() const;
        virtual void setDataSource(TracerDataSource *dataSource);

        virtual TracerDelegate * delegate() const;
        virtual void setDelegate(TracerDelegate *delegate);

    private:
        TracerDataSource *m_dataSource;
        TracerDelegate *m_delegate;
    };

}

#endif /* TracerProtocol_hpp */
