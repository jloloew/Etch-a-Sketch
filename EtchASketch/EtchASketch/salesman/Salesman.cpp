//
//  Salesman.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "Salesman.hpp"
#include "EASUtils.hpp"

using std::unordered_set;
using std::vector;
using etchasketch::KDTree;
using etchasketch::Point;

etchasketch::salesman::Salesman::Salesman(const unordered_set<Point<2> *> &unorderedPoints)
: unorderedPoints(unorderedPoints),
orderedPoints(vector<Point<2>>(unorderedPoints.size()))
{ }

etchasketch::salesman::Salesman::~Salesman(void)
{ }

void
etchasketch::salesman::Salesman::orderPoints(void)
{
	// Create a K-D tree with the points.
	KDTree<2> *kdTree = new KDTree<2>(unorderedPoints);
	
	primsAlgorithm(*kdTree);
	
	delete kdTree;
}

void
etchasketch::salesman::Salesman::primsAlgorithm(KDTree<2> &kdTree)
{
	// Assume that we start at (0, 0).
	// TODO: Fix needing to use doubles in this constructor.
	const Point<2> startPoint = Point<2>(0.0, 0.0);
	orderedPoints.push_back(startPoint);
	
	while (!unorderedPoints.empty()) {
		_EASLog("Size remaining: %lu", unorderedPoints.size());
		// Find the next point nearest the last point we added, add it to the
		// list of ordered points, and remove it as an option in the list of
		// unordered point list.
		Point<2> *currPoint = kdTree.findNearestNeighbor(orderedPoints.back());
		if ((currPoint != nullptr) && currPoint->isValid()) {
			orderedPoints.push_back(*currPoint);
			
			// erase returns the number of elements removed.
			if (1 != unorderedPoints.erase(currPoint)) {
				_EASLog("Failed to remove the current value.");
			}
			// When the KD tree removes a point, it also deletes it.
			kdTree.remove(currPoint);
		} else {
			_EASLog("Bad currPoint.");
		}
	}
}
