//
//  TracerProtocol.cpp
//  Tracer
//
//  Created by Justin Loew on 1/3/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#include "TracerProtocol.hpp"

tracer::TracerDefaults::TracerDefaults()
: m_dataSource(nullptr), m_delegate(nullptr)
{}

tracer::TracerDataSource *
tracer::TracerDefaults::dataSource() const
{
    return m_dataSource;
}

void
tracer::TracerDefaults::setDataSource(tracer::TracerDataSource *dataSource)
{
    m_dataSource = dataSource;
}

tracer::TracerDelegate *
tracer::TracerDefaults::delegate() const
{
    return m_delegate;
}

void
tracer::TracerDefaults::setDelegate(tracer::TracerDelegate *delegate)
{
    m_delegate = delegate;
}
