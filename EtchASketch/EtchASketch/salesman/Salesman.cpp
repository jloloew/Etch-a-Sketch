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
using etchasketch::KDPoint;

etchasketch::salesman::Salesman::Salesman(const unordered_set<KDPoint<2>> &unorderedPoints,
										  const KDPoint<2> &startPoint)
: startPoint(startPoint),
unorderedPoints(unorderedPoints),
orderedPoints(vector<KDPoint<2>>())//unorderedPoints.size()))
{ }

etchasketch::salesman::Salesman::~Salesman(void)
{ }

void
etchasketch::salesman::Salesman::orderPoints(void)
{
	// Create a K-D tree with the points.
	KDTree<2> *kdTree = new KDTree<2>(unorderedPoints);
	
	nearestNeighborAlgorithm(*kdTree);
	
	delete kdTree;
}

void
etchasketch::salesman::Salesman::nearestNeighborAlgorithm(KDTree<2> &kdTree)
{
	// Assume that we start at (0, 0).
	const KDPoint<2> startPoint(0, 0);
	orderedPoints.push_back(startPoint);
	kdTree.remove(startPoint);
	
	while (!unorderedPoints.empty()) {
//		EASLog("Size remaining: %lu", unorderedPoints.size());
		
		// Find the next point nearest the last point we added, add it to the
		// list of ordered points, and remove it as an option in the list of
		// unordered point list.
		KDPoint<2> *currPoint =
				kdTree.findNearestNeighbor(orderedPoints.back());
		if ((nullptr != currPoint) && currPoint->isValid()) {
			orderedPoints.push_back(*currPoint);
			
			// erase returns the number of elements removed.
			if (1 != unorderedPoints.erase(*currPoint)) {
				EASLog("Failed to remove the current value.");
			}
			// Remove the point from the KDTree and delete our copy of it.
			kdTree.remove(*currPoint);
			delete currPoint;
		} else {
			EASLog("KDTree.findNearestNeighbor returned a bad currPoint.");
		}
	}
}
