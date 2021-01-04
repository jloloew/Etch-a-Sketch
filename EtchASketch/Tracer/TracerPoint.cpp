//
//  TracerPoint.cpp
//  Tracer
//
//  Created by Justin Loew on 1/3/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#include "TracerPoint.hpp"

#include <stdexcept>

using tracer::Dimension;
using tracer::PointCoordinate;

/* KDPoint constructor. Initializes everything to allValues.
 */
tracer::Point::Point(const PointCoordinate allValues)
: Point(allValues, allValues)
{}

tracer::Point::Point(const PointCoordinate x, const PointCoordinate y)
: x(x), y(y)
{}

PointCoordinate
tracer::Point::operator[](Dimension index) const
{
    switch (index) {
        case X:
            return x;
        case Y:
            return y;
        default:
            throw std::out_of_range("Point index out of range");
    }
}

PointCoordinate &
tracer::Point::operator[](Dimension index)
{
    switch (index) {
        case X:
            return x;
        case Y:
            return y;
        default:
            throw std::out_of_range("Point index out of range");
    }
}

void
tracer::Point::print(std::ostream &out) const
{
    out << '(' << x << ", " << y << ')';
}

std::ostream &
tracer::operator<<(std::ostream &out, const tracer::Point &p)
{
    p.print(out);
    return out;
}

bool
tracer::Point::operator==(const Point p) const
{
    return !(*this != p);
}

bool
tracer::Point::operator!=(const Point p) const
{
    return (x != p.x) || (y != p.y);
}

size_t
std::hash<tracer::Point>::operator()(const tracer::Point &pt) const
{
    // Hash the point.
    return (pt.x * 0x1f1f1f1f) ^ pt.y;
}

size_t
std::hash<const tracer::Point>::operator()(const tracer::Point &pt) const
{
    // Hash the point.
    return (pt.x * 0x1f1f1f1f) ^ pt.y;
}
