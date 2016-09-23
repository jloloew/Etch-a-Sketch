//
//  Point.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "Point.hpp"

/* point.cpp
	Implementation of Point class
 
	Written by Matt Sachtler for CS225 MP6
	Spring 2009
	Revision history:
 3/21/2009		Created
 */

using etchasketch::PointCoordinate;

/* Point constructor. Initializes everything to 0.
 */
template<int Dim>
etchasketch::Point<Dim>::Point()
{
	for(int i = 0; i < Dim; i++) {
		vals[i] = 0;
	}
}

/* Point constructor
	Copy the array of points in
 */
template<int Dim>
etchasketch::Point<Dim>::Point(PointCoordinate arr[Dim])
{
	for(int i = 0; i < Dim; i++) {
		vals[i] = arr[i];
	}
}

template<int Dim>
template <typename T>
etchasketch::Point<Dim>::Point(T x0, T x1, T x2)
{
	vals[0] = x0;
	vals[1] = x1;
	vals[2] = x2;
}

template<int Dim>
template <typename T>
etchasketch::Point<Dim>::Point(T x, ...)
{
	vals[0] = x;
	va_list ap;
	va_start(ap, x);
	for (int i = 1; i < Dim; i++) {
		vals[i] = va_arg(ap, T);
	}
	va_end(ap);
}

template<int Dim>
bool
etchasketch::Point<Dim>::isValid(void) const
{
	PointCoordinate invalid = etchasketch::Point<Dim>::PointCoordinateInvalid;
	for (int i = 0; i < Dim; i++) {
		if (vals[i] == invalid) {
			return false;
		}
	}
	return true;
}

template<int Dim>
bool
etchasketch::Point<Dim>::isLeaf(void) const
{
	return (lesserPoints == nullptr) && (greaterPoints == nullptr);
}

template<int Dim>
float
etchasketch::Point<Dim>::distanceTo(const Point<Dim> &other) const
{
	float dist = 0.0f;
	for (int i = 0; i < Dim; i++) {
		float diff = static_cast<float>((*this)[i] - other[i]);
		dist += diff * diff;
	}
	return dist;
}

template<int Dim>
PointCoordinate
etchasketch::Point<Dim>::operator[](int index) const
{
	if (index >= Dim) {
		out_of_range e("Point index out of range");
		throw e;
	}
	return vals[index];
}

template<int Dim>
PointCoordinate &
etchasketch::Point<Dim>::operator[](int index)
{
	if (index >= Dim) {
		out_of_range e("Point index out of range");
		throw e;
	}
	return vals[index];
}

template<int Dim>
void
etchasketch::Point<Dim>::set(int index, PointCoordinate val)
{
	if (index >= Dim) {
		out_of_range e("Point index out of range");
		throw e;
	}
	vals[index] = val;
}

template<int Dim>
void
etchasketch::Point<Dim>::print(std::ostream &out /* = cout */) const
{
	out << "{value: ";
	printVals(out);
	out << ", lesserPoints: ";
	out << (lesserPoints != nullptr) ?: "NULL";
	out << ", greaterPoints: ";
	out << (greaterPoints != nullptr) ?: "NULL";
	out << "}";
}

template<int Dim>
void
etchasketch::Point<Dim>::printVals(std::ostream &out) const
{
	out << '(';
	
	for (int i = 0; i < Dim - 1; i++) {
		out << vals[i] << ", ";
	}
	out << vals[Dim - 1];
	
	out << ')';
}

template<int Dim>
std::ostream &
operator<<(std::ostream &out, const etchasketch::Point<Dim> &p)
{
	p.print(out);
	return out;
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator==(const etchasketch::Point<Dim> p) const
{
	return !(*this != p);
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator!=(const etchasketch::Point<Dim> p) const
{
	bool eq = true;
	for (int i = 0; i < Dim; i++) {
		eq &= (vals[i] == p.vals[i]);
	}
	return !eq;
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator<(const etchasketch::Point<Dim> p) const
{
	bool less = false;
	for (int i = 0; i < Dim; i++) {
		less = vals[i] < p[i];
		if (vals[i] != p[i]) {
			break;
		}
	}
	return less;
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator<=(const etchasketch::Point<Dim> p) const
{
	return (*this < p) || (*this == p);
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator>(const etchasketch::Point<Dim> p) const
{
	return !(*this < p) && (*this != p);
}

template<int Dim>
bool
etchasketch::Point<Dim>::operator>=(const etchasketch::Point<Dim> p) const
{
	return (*this > p) || (*this == p);
}
