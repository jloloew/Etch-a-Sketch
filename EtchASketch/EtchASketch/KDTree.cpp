//
//  KDTree.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#include "KDTree.hpp"
#include <algorithm>

using std::unordered_set;
using std::vector;
using etchasketch::Point;


#pragma mark - Constructors

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const unordered_set<Point<Dim> *> &newPoints)
{
	if (newPoints.empty()) {
		root = nullptr;
		return;
	}
	
	buildTree(newPoints);
}

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const vector<Point<Dim> *> &newPoints)
: KDTree(unordered_set<Point<Dim> *>(newPoints.begin(), newPoints.end()))
{ }

template<int Dim>
void
etchasketch::KDTree<Dim>::buildTree(const vector<Point<Dim> *> &points)
{
	// TODO: Would sorting the points help speed up the build?
	// Just insert each point.
	for (auto pointIter = points.begin(); pointIter != points.end(); pointIter++) {
		insert(*pointIter);
	}
}

template<int Dim>
etchasketch::KDTree<Dim>::~KDTree()
{
	delete root;
}

#pragma mark -

template<int Dim>
bool
etchasketch::KDTree<Dim>::smallerDimVal(const Point<Dim> &first,
										const Point<Dim> &second,
										const int curDim) const
{
	if (first[curDim] < second[curDim]) {
		return true;
	} else if (first[curDim] > second[curDim]) {
		return false;
	} else {
		return first < second;
	}
}

#pragma mark - Find nearest neighbor

template<int Dim>
Point<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const Point<Dim> &query) const
{
	double currentBestDist = 1.0 / 0.0; // infinity
	return findNearestNeighbor(query, root, currentBestDist, 0);
}

template<int Dim>
Point<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const Point<Dim> &query,
											  const etchasketch::Point<Dim> &subRoot,
											  double &currentBestDist, // inout
											  const int dimension) const
{
	// Base cases.
	if (subRoot == nullptr) {
		return nullptr;
	}
	if (subRoot.isLeaf()) {
		currentBestDist = query.distanceTo(subRoot);
		return subRoot;
	}
	
	const int nextDimension = (dimension + 1) % Dim;
	const double myDistance = query.distanceTo(subRoot);
	const Point<Dim> *currentBestPoint = nullptr;
	
	// Recurse to find the nearest neighbor in a subtree.
	if (smallerDimVal(query, subRoot, dimension)) {
		// Go left.
		currentBestPoint = findNearestNeighbor(query, subRoot.lesserPoints, currentBestDist, nextDimension);
	} else {
		// Go right.
		currentBestPoint = findNearestNeighbor(query, subRoot.greaterPoints, currentBestDist, nextDimension);
	}
	
	// Check if the subroot is better than the subtree's best.
	if (shouldReplace(query, currentBestPoint, subRoot)) {
		currentBestDist = myDistance;
		currentBestPoint = subRoot;
	}
	
	// Check if there could be a better point in the other subtree.
	// If the difference between query and subroot in the current dimension is
	// less than that between query and the other node, it's possible that the
	// other subtree contains a closer node.
	const double diffInCurDim = (query[dimension] - subRoot[dimension]);
	const double distInCurDim = diffInCurDim * diffInCurDim;
	if (distInCurDim <= currentBestDist) {
		double newBestDist = 1.0 / 0.0; // Infinity.
		Point<Dim> *newBestPoint;
		if (smallerDimVal(subRoot, query, dimension)) {
			// Go left.
			newBestPoint = findNearestNeighbor(query, subRoot.lesserPoints, newBestDist, nextDimension);
		} else {
			// Go right.
			newBestPoint = findNearestNeighbor(query, subRoot.greaterPoints, newBestDist, nextDimension);
		}
		
		if (newBestPoint != nullptr) {
			// Check the distance we just got.
			if (shouldReplace(query, currentBestPoint, newBestPoint)) {
				currentBestDist = newBestDist;
				currentBestPoint = newBestPoint;
			}
		}
	}
	
	return currentBestPoint;
}

template<int Dim>
bool
etchasketch::KDTree<Dim>::shouldReplace(const Point<Dim> &target,
										const Point<Dim> *currentBest,
										const Point<Dim> &potential) const
{
	if (currentBest == nullptr) {
		return true;
	}
	
	const double currentBestDist = target.distanceTo(*currentBest);
	const double potentialDist = target.distanceTo(potential);
	
	if (potentialDist < currentBestDist) {
		return true;
	} else if (potentialDist > currentBestDist) {
		return false;
	} else {
		return potential < currentBest;
	}
}

#pragma mark -

template<int Dim>
bool
etchasketch::KDTree<Dim>::contains(const Point<Dim> &query) const
{
	const Point<Dim> *subRoot = *root;
	int dimension = 0;
	while (subRoot != nullptr) {
		// Base case.
		if (*subRoot == query) {
			return true;
		}
		
		if (smallerDimVal(query, *subRoot, dimension)) {
			// Go left.
			subRoot = subRoot->lesserPoints;
		} else {
			// Go right.
			subRoot = subRoot->greaterPoints;
		}
		dimension = (dimension + 1) % Dim;
	}
	return false;
}

#pragma mark - Tree modification

template<int Dim>
void
etchasketch::KDTree<Dim>::insert(Point<Dim> &newPoint)
{
	// Temporarily remove the subtrees.
	Point<Dim> *left = newPoint.lesserPoints;
	Point<Dim> *right = newPoint.greaterPoints;
	newPoint.lesserPoints = nullptr;
	newPoint.greaterPoints = nullptr;
	
	if (root != nullptr) {
		// Call the helper.
		insert(newPoint, *root, 0);
	} else {
		root = &newPoint;
	}
	
	// Insert the subtrees recursively.
	if (left != nullptr) {
		insert(*left);
	}
	if (right != nullptr) {
		insert(*right);
	}
}

template<int Dim>
void
etchasketch::KDTree<Dim>::insert(Point<Dim> &newPoint,
								 Point<Dim> &subRoot,
								 const int dimension)
{
	const int nextDimension = (dimension + 1) % Dim;
	// Should we go left or right?
	if (smallerDimVal(newPoint, subRoot, dimension)) {
		// Go left.
		Point<Dim> *lesser = subRoot.lesserPoints;
		if (lesser != nullptr) {
			insert(newPoint, *lesser, nextDimension);
		} else {
			// Put the new point here.
			subRoot.lesserPoints = &newPoint;
			// TODO: Clear the lesserPoints and greaterPoints of newPoint?
		}
	} else {
		// Go right.
		Point<Dim> *greater = subRoot.greaterPoints;
		if (greater != nullptr) {
			insert(newPoint, *greater, nextDimension);
		} else {
			// Put the new point here.
			subRoot.greaterPoints = &newPoint;
			// TODO: Clear the lesserPoints and greaterPoints of newPoint?
		}
	}
}

template<int Dim>
void
etchasketch::KDTree<Dim>::remove(Point<Dim> *&targetPoint)
{
	// Safety first.
	if (targetPoint == nullptr) {
		return;
	}
	
	// Get the parent point.
	Point<Dim> *parent = getParent(*targetPoint);
	if (parent != nullptr) {
		// Set the parent's reference to the target point to NULL, effectively
		// removing it from the tree.
		if (parent->lesserPoints == targetPoint) {
			targetPoint = parent->lesserPoints;
			parent->lesserPoints = nullptr;
		} else if (parent->greaterPoints == targetPoint) {
			targetPoint = parent->greaterPoints;
			parent->greaterPoints = nullptr;
		}
	} else if (targetPoint == root) {
		// The target is the root node.
		targetPoint = root;
		root = nullptr;
	}
	
	// Reinsert each of its children into the tree.
	if (targetPoint->lesserPoints != nullptr) {
		insert(targetPoint->lesserPoints);
		targetPoint->lesserPoints = nullptr;
	}
	if (targetPoint->greaterPoints != nullptr) {
		insert(targetPoint->greaterPoints);
		targetPoint->lesserPoints = nullptr;
	}
	
	// Delete the now-removed node.
	delete targetPoint;
	targetPoint = nullptr;
}

template<int Dim>
const Point<Dim> *
etchasketch::KDTree<Dim>::getParent(const etchasketch::Point<Dim> &child) const
{
	int dimension = 0;
	const Point<Dim> *subRoot = root;
	while (subRoot != nullptr) {
		// Base case.
		if (subRoot->isLeaf()) {
			// No parent exists.
			return nullptr;
		}
		if ((*subRoot->lesserPoints == child) || (*subRoot->greaterPoints == child)) {
			return subRoot;
		}
		
		if (smallerDimVal(child, *subRoot, dimension)) {
			// Go left.
			subRoot = subRoot->lesserPoints;
		} else {
			// Go right.
			subRoot = subRoot->greaterPoints;
		}
		dimension = (dimension + 1) % Dim;
	}
	return nullptr;
}

template<int Dim>
void
etchasketch::KDTree<Dim>::print(std::ostream &out) const
{
	// Just print all the nodes.
	if (root != nullptr) {
		out << *root;
	} else {
		out << "Empty";
	}
}

template<int Dim>
std::ostream &
etchasketch::operator<<(std::ostream &out,
						const etchasketch::KDTree<Dim> &kdtree)
{
	kdtree.print(out);
	return out;
}
