//
//  KDPoint.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef KDPoint_hpp
#define KDPoint_hpp

#include <cstdarg>
#include <iostream>
#include <stdexcept>

using std::out_of_range;
using std::cout;
using std::endl;

namespace etchasketch {

	typedef int KDPointCoordinate;
	
	/**
	 * KDPoint class: represents a point in Dim dimensional space.
	 *
	 * @author Matt Sachtler
	 * @date Spring 2009
	 */
	template<int Dim>
	struct KDPoint
	{
	public:
		static const KDPointCoordinate KDPointCoordinateInvalid = INT_MAX;
		
	private:
		KDPointCoordinate vals[Dim];
		
	public:
		/// The subtree containing all points less than the current point.
		KDPoint<Dim> *lesserPoints;
		
		/// The subtree containing all points greater than the current point.
		KDPoint<Dim> *greaterPoints;
		
		KDPoint(const KDPointCoordinate allValues = 0);
		
		KDPoint(KDPointCoordinate arr[Dim]);
		
		explicit KDPoint(KDPointCoordinate x, KDPointCoordinate y ...);
		
		/**
		 * Copy constructor.
		 * Copies the values from other, but *not* the subtree pointers.
		 */
		KDPoint(const KDPoint<Dim> & __restrict other);
		
		inline
		bool isValid() const;
		
		inline
		bool isLeaf() const;
		
		/**
		 * @param other The point to measure the distance to.
		 * @return The square of the distance to other.
		 */
		float distanceTo(const KDPoint<Dim> &other) const;
		
		/**
		 * Gets the value of the KDPoint object in the given dimension
		 * (index). This function is const so we don't modify KDPoints using
		 * this function.
		 *
		 * @param index The dimension of the KDPoint to grab.
		 * @return The value of the KDPoint in the indexth dimension.
		 */
		KDPointCoordinate operator[](int index) const;
		
		/**
		 * Gets the value of the KDPoint object in the given dimension
		 * (index). This is the non-const version, so it can be used to
		 * modify KDPoints like so:
		 *
		 *     KDPoint<3> a(1, 2, 3);
		 *     a[0] = 4; // a is now (4, 2, 3)
		 *
		 * @param index The dimension of the KDPoint to grab.
		 * @return The value of the KDPoint in the indexth dimension, by
		 *	reference (so that it may be modified).
		 */
		KDPointCoordinate & operator[](int index);
		
		bool operator==(const KDPoint<Dim> p) const;
		bool operator!=(const KDPoint<Dim> p) const;
		
		/**
		 * Compares whether the given KDPoint is smaller than the current
		 * KDPoint.
		 *
		 * @param p The other point to compare with.
		 * @return A boolean value indicating whether the current KDPoint is
		 *  smaller than the provided KDPoint p.
		 */
		bool operator<(const KDPoint<Dim> p) const;
		
		bool operator<=(const KDPoint<Dim> p) const;
		bool operator>(const KDPoint<Dim> p) const;
		bool operator>=(const KDPoint<Dim> p) const;
		
		void set(int index, KDPointCoordinate val);
		
		void print(std::ostream &out = std::cout) const;
		
	private:
		void printVals(std::ostream &out = std::cout) const;
	};
	
	template<int Dim>
	std::ostream & operator<<(std::ostream &out, const KDPoint<Dim> &p);
}

// Add a hash function for use in unordered_map.
namespace std {
	// Non-const version.

	template<>
    struct hash<etchasketch::KDPoint<2>> {
		size_t
		operator()(const etchasketch::KDPoint<2> &pt) const
		{
			// Hash the point.
			return (pt[0] * 0x1f1f1f1f) ^ pt[1];
		}
	};

    //const etchasketch::KDPoint<2>
	
	// Const version.
	template<>
    struct hash<const etchasketch::KDPoint<2>> {
		size_t
		operator()(const etchasketch::KDPoint<2> &pt) const
		{
			// Hash the point.
			return (pt[0] * 0x1f1f1f1f) ^ pt[1];
		}
	};
}

#include "KDPoint.cpp"

#endif /* KDPoint_hpp */
