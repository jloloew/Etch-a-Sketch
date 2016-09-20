//
//  KDTree.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

//#include "KDTree.hpp"
#include <algorithm>

using std::vector;

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first,
								const Point<Dim> & second,
								int curDim) const
{
	if (first[curDim] < second[curDim]) {
		return true;
	} else if (first[curDim] > second[curDim]) {
		return false;
	} else {
		return first < second;
	}
}


template<int Dim> inline
double KDTree<Dim>::distance(const Point<Dim> & first, const Point<Dim> & second) const
{
	double dist = 0.0;
	for (int i = 0; i < Dim; i++)
		dist += (first[i] - second[i]) * (first[i] - second[i]);
	return dist;
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target,
								const Point<Dim> & currentBest,
								const Point<Dim> & potential) const
{
	double currentBestDist = distance(target, currentBest);
	double potentialDist = distance(target, potential);
	
	if (potentialDist < currentBestDist)
		return true;
	else if (potentialDist > currentBestDist)
		return false;
	else
		return potential < currentBest;
}


template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
	if (newPoints.empty()) {
		points = vector< Point<Dim> >();
		return;
	}
	
	points.assign(newPoints.begin(), newPoints.end());
	
	buildTree(0, newPoints.size()-1, 0);
}


template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const
{
	double currentBestDist = 1.0 / 0.0; // infinity
	return findNearestNeighbor(query, currentBestDist, 0, 0, points.size()-1);
}


template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query,
											double & currentBestDist, // inout
											const int dimension,
											const size_t lower,
											const size_t upper) const
{
	// base case
	if (lower < 0 || upper >= points.size())
	{
		// return an obviously invalid point
		Point<Dim> pt;
		for (int i = 0; i < Dim; i++)
			pt[i] = 1.0 / 0.0; // infinity
		return pt;
	}
	if (upper < lower)
	{
		if (lower < points.size())
		{
			currentBestDist = distance(query, points.at(lower));
			return points.at(lower);
		}
		else
		{
			currentBestDist = distance(query, points.at(upper));
			return points.at(upper);
		}
	}
	
	const int nextDimension    = (dimension + 1) % Dim;
	const size_t medianIndex   = (lower + upper) / 2;
	const Point<Dim> & subroot = points.at(medianIndex);
	const double myDist        = distance(query, points.at(medianIndex));
	Point<Dim> currentBestPoint;
	
	// recurse to find the nearest neighbor in a subtree
	if (smallerDimVal(subroot, query, dimension))
	{
		// go right
		currentBestPoint = findNearestNeighbor(query,
											   currentBestDist,
											   nextDimension,
											   medianIndex+1,
											   upper);
	}
	else
	{
		// go left
		currentBestPoint = findNearestNeighbor(query,
											   currentBestDist,
											   nextDimension,
											   lower,
											   medianIndex-1);
	}
	
	
	
	// check if this node is better than the subtree's best
	if (shouldReplace(query, currentBestPoint, subroot))
	{
		currentBestDist  = myDist;
		currentBestPoint = subroot;
	}
	
	// Check if there could be a better node in the other subtree.
	// If the difference between query and subroot in the current dimension
	// is less than that between query and the other node, it's possible
	// that the other subtree contains a closer node.
	if ((query[dimension] - subroot[dimension]) * (query[dimension] - subroot[dimension]) <= currentBestDist)
	{
		double newBestDist = 1.0 / 0.0; // infinity
		Point<Dim> newBestPoint;
		if (smallerDimVal(subroot, query, dimension))
		{
			// go left
			newBestPoint = findNearestNeighbor(query,
											   newBestDist,
											   nextDimension,
											   lower,
											   medianIndex-1);
		}
		else
		{
			// go right
			newBestPoint = findNearestNeighbor(query,
											   newBestDist,
											   nextDimension,
											   medianIndex+1,
											   upper);
		}
		
		// check the distance we just got
		if (shouldReplace(query, currentBestPoint, newBestPoint)) {
			currentBestDist  = newBestDist;
			currentBestPoint = newBestPoint;
		}
	}
	
	return currentBestPoint;
}


template<int Dim>
void KDTree<Dim>::buildTree(int left, int right, int dimension)
{
	if (left >= right)
		return;
	int mid = (left + right) / 2;
	select(left, right, mid, dimension);
	int nextDimension = (dimension + 1) % Dim;
	buildTree(left, mid-1, nextDimension);
	buildTree(mid+1, right, nextDimension);
}


template<int Dim>
int KDTree<Dim>::partition(int left, int right, int pivotIndex, int dimension)
{
	std::swap(points.at(pivotIndex), points.at(right));	// move pivot to end
	pivotIndex = right;
	int storeIndex = left;
	for (int i = left; i < right; i++)
		if (smallerDimVal(points.at(i), points.at(pivotIndex), dimension))
			std::swap(points.at(storeIndex++), points.at(i));
	std::swap(points.at(right), points.at(storeIndex)); // move pivot to its final place
	return storeIndex;
}


template<int Dim>
int KDTree<Dim>::select(int left, int right, int n, int dimension)
{
	if (left == right) // if the list contains only one element, return that element
		return left;
	int pivotIndex = partition(left, right, n, dimension);
	// the pivot is in its final sorted position
	if (n == pivotIndex)
		return n;
	else if (n < pivotIndex)
		return select(left, pivotIndex-1, n, dimension);
	else
		return select(pivotIndex+1, right, n, dimension);
}
