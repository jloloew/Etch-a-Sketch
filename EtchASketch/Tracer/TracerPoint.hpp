//
//  TracerPoint.hpp
//  Tracer
//
//  Created by Justin Loew on 1/3/21.
//  Copyright © 2021 Justin Loew. All rights reserved.
//

#ifndef TracerPoint_hpp
#define TracerPoint_hpp

#include <climits>
#include <iostream>

namespace tracer {

    /// Represents a coordinate in a single dimension.
    typedef int PointCoordinate;

    typedef enum Dimension {
        X = 0,
        Y = 1,
        kNumDimensions = Y + 1
    } Dimension;

    struct Point
    {
    public:
        /// A value representing an invalid coordinate.
        static const PointCoordinate PointCoordinateInvalid = INT_MAX;
    public:
        PointCoordinate x, y;

        Point(const PointCoordinate allValues = 0);

        explicit Point(PointCoordinate x, PointCoordinate y);

        /**
         * Gets the value of the Point object in the given dimension
         * (index). This function is const so we don't modify Points using
         * this function.
         *
         * @param index The dimension of the Point to grab.
         * @return The value of the Point in the indexth dimension.
         */
        PointCoordinate operator[](Dimension index) const;

        /**
         * Gets the value of the Point object in the given dimension
         * (index). This is the non-const version, so it can be used to
         * modify Points like so:
         *
         *     Point a(1, 2);
         *     a[0] = 4; // a is now (4, 2)
         *
         * @param index The dimension of the Point to grab.
         * @return The value of the Point in the indexth dimension, by
         *    reference (so that it may be modified).
         */
        PointCoordinate & operator[](Dimension index);

        bool operator==(const Point p) const;
        bool operator!=(const Point p) const;

        /// Print a textual representation of the point to an output stream.
        void print(std::ostream &out = std::cout) const;
    };

    std::ostream & operator<<(std::ostream &out, const Point &p);

}

// Add a hash function for use in unordered_map.
namespace std {
    /// Hash a point.
    template<>
    struct hash<tracer::Point> {
        size_t
        operator()(const tracer::Point &pt) const;
    };

    /// Hash a point.
    template<>
    struct hash<const tracer::Point> {
        size_t
        operator()(const tracer::Point &pt) const;
    };
}

#endif /* TracerPoint_hpp */
