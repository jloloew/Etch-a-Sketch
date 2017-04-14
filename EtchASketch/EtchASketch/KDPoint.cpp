//
//  KDPoint.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

//#include "KDPoint.hpp"

/* point.cpp
	Implementation of KDPoint class
 
	Written by Matt Sachtler for CS225 MP6
	Spring 2009
	Revision history:
 3/21/2009		Created
 */

using etchasketch::KDPointCoordinate;

/* KDPoint constructor. Initializes everything to allValues.
 */
template<int Dim>
etchasketch::KDPoint<Dim>::KDPoint(const KDPointCoordinate allValues)
{
	this->lesserPoints = nullptr;
	this->greaterPoints = nullptr;
	for (int i = 0; i < Dim; i++) {
		vals[i] = allValues;
	}
}

/* KDPoint constructor
	Copy the array of points in
 */
template<int Dim>
etchasketch::KDPoint<Dim>::KDPoint(const KDPointCoordinate arr[Dim])
: KDPoint()
{
	for (int i = 0; i < Dim; i++) {
		vals[i] = arr[i];
	}
}

template<int Dim>
etchasketch::KDPoint<Dim>::KDPoint(KDPointCoordinate x, KDPointCoordinate y ...)
: KDPoint()
{
	vals[0] = x;
	vals[1] = y;
	va_list ap;
	va_start(ap, y);
	for (int i = 2; i < Dim; i++) {
		vals[i] = va_arg(ap, KDPointCoordinate);
	}
	va_end(ap);
}

template<int Dim>
etchasketch::KDPoint<Dim>::KDPoint(const KDPoint<Dim> & __restrict other)
: KDPoint()
{
	memcpy(this->vals, other.vals, sizeof(this->vals));
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::isValid() const
{
	KDPointCoordinate invalid = etchasketch::KDPoint<Dim>::KDPointCoordinateInvalid;
	for (int i = 0; i < Dim; i++) {
		if (vals[i] == invalid) {
			return false;
		}
	}
	return true;
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::isLeaf() const
{
	return (lesserPoints == nullptr) && (greaterPoints == nullptr);
}

template<int Dim>
float
etchasketch::KDPoint<Dim>::distanceTo(const KDPoint<Dim> &other) const
{
	float dist = 0.0f;
	for (int i = 0; i < Dim; i++) {
		float diff = static_cast<float>((*this)[i] - other[i]);
		dist += diff * diff;
	}
	return dist;
}

template<int Dim>
KDPointCoordinate
etchasketch::KDPoint<Dim>::operator[](int index) const
{
	if (index < 0 || index >= Dim) {
		out_of_range e("KDPoint index out of range");
		throw e;
	}
	return vals[index];
}

template<int Dim>
KDPointCoordinate &
etchasketch::KDPoint<Dim>::operator[](int index)
{
	if (index < 0 || index >= Dim) {
		out_of_range e("KDPoint index out of range");
		throw e;
	}
	return vals[index];
}

template<int Dim>
void
etchasketch::KDPoint<Dim>::set(int index, KDPointCoordinate val)
{
	if (index < 0 || index >= Dim) {
		out_of_range e("KDPoint index out of range");
		throw e;
	}
	vals[index] = val;
}

template<int Dim>
void
etchasketch::KDPoint<Dim>::print(std::ostream &out /* = cout */) const
{
	out << "{\"value\": ";
	printVals(out);
	out << ", \"lesserPoints\": ";
	if (nullptr != lesserPoints) {
		lesserPoints->print(out);
	} else {
		out << "null";
	}
	out << ", \"greaterPoints\": ";
	if (nullptr != greaterPoints) {
		greaterPoints->print(out);
	} else {
		out << "null";
	}
	out << "}";
}

template<int Dim>
void
etchasketch::KDPoint<Dim>::printVals(std::ostream &out) const
{
	out << '[' << vals[0];
	
	for (int i = 1; i < Dim; i++) {
		out << ", " << vals[i];
	}
	
	out << ']';
}

template<int Dim>
std::ostream &
etchasketch::operator<<(std::ostream &out, const etchasketch::KDPoint<Dim> &p)
{
	p.print(out);
	return out;
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::operator==(const etchasketch::KDPoint<Dim> p) const
{
	return !(*this != p);
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::operator!=(const etchasketch::KDPoint<Dim> p) const
{
	bool eq = true;
	for (int i = 0; i < Dim; i++) {
		eq &= (vals[i] == p.vals[i]);
	}
	return !eq;
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::operator<(const etchasketch::KDPoint<Dim> p) const
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
etchasketch::KDPoint<Dim>::operator<=(const etchasketch::KDPoint<Dim> p) const
{
	return (*this < p) || (*this == p);
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::operator>(const etchasketch::KDPoint<Dim> p) const
{
	return !(*this < p) && (*this != p);
}

template<int Dim>
bool
etchasketch::KDPoint<Dim>::operator>=(const etchasketch::KDPoint<Dim> p) const
{
	return (*this > p) || (*this == p);
}
