//
//  KDTree.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

//#include "KDTree.hpp"
#include <algorithm>
#include <sstream>
#include "EASUtils+Private.hpp"

using std::stringstream;
using std::unordered_set;
using std::vector;
using etchasketch::KDPoint;


#pragma mark - Constructors

template<int Dim>
etchasketch::KDTree<Dim>::KDTree()
: root(nullptr)
{ }

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const unordered_set<KDPoint<Dim>> &newPoints)
: KDTree()
{
	if (newPoints.empty()) {
		return;
	}
	
	buildTree(newPoints);
}

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const vector<KDPoint<Dim>> &newPoints)
: KDTree(unordered_set<KDPoint<Dim>>(newPoints.begin(), newPoints.end()))
{ }

template<int Dim>
void
etchasketch::KDTree<Dim>::buildTree(const unordered_set<KDPoint<Dim>> &points)
{
	// TODO: Would sorting the points help speed up the build?
	// Just insert each point.
	for (auto ptIter = points.begin(); ptIter != points.end(); ++ptIter) {
		// Make a copy of the points.
		KDPoint<Dim> *point = new KDPoint<Dim>(*ptIter);
		this->insert(point);
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
etchasketch::KDTree<Dim>::smallerDimVal(const KDPoint<Dim> &first,
										const KDPoint<Dim> &second,
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
KDPoint<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const KDPoint<Dim> &query) const
{
	double currentBestDist = 1.0 / 0.0; // Infinity.
	const KDPoint<Dim> *nn = findNearestNeighbor(query, root, currentBestDist,
												 0);
	if (nullptr != nn) {
		// Return a copy of the nearest neighbor.
		return new KDPoint<Dim>(*nn);
	}
	return nullptr;
}

template<int Dim>
const KDPoint<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const KDPoint<Dim> &query,
											  const KDPoint<Dim> *subroot,
											  double &currentBestDist, // inout
											  const int dimension) const
{
	// Base cases.
	if (nullptr == subroot) {
		return nullptr;
	}
	const KDPoint<Dim> &subRoot = *subroot;
	if (subRoot.isLeaf()) {
		currentBestDist = query.distanceTo(subRoot);
		return subroot;
	}
	
	const int nextDimension = (dimension + 1) % Dim;
	const double myDistance = query.distanceTo(subRoot);
	const KDPoint<Dim> *currentBestKDPoint = nullptr;
	
	// Recurse to find the nearest neighbor in a subtree.
	if (smallerDimVal(query, subRoot, dimension)) {
		// Go left.
		currentBestKDPoint = findNearestNeighbor(query, subRoot.lesserPoints, currentBestDist, nextDimension);
	} else {
		// Go right.
		currentBestKDPoint = findNearestNeighbor(query, subRoot.greaterPoints, currentBestDist, nextDimension);
	}
	
	// Check if the subroot is better than the subtree's best.
	if (shouldReplace(query, currentBestKDPoint, subRoot)) {
		currentBestDist = myDistance;
		currentBestKDPoint = subroot;
	}
	
	// Check if there could be a better point in the other subtree.
	// If the difference between query and subroot in the current dimension is
	// less than that between query and the other node, it's possible that the
	// other subtree contains a closer node.
	const double diffInCurDim = (query[dimension] - subRoot[dimension]);
	const double distInCurDim = diffInCurDim * diffInCurDim;
	if (distInCurDim <= currentBestDist) {
		double newBestDist = 1.0 / 0.0; // Infinity.
		const KDPoint<Dim> *newBestKDPoint;
		if (smallerDimVal(subRoot, query, dimension)) {
			// Go left.
			newBestKDPoint = findNearestNeighbor(query, subRoot.lesserPoints, newBestDist, nextDimension);
		} else {
			// Go right.
			newBestKDPoint = findNearestNeighbor(query, subRoot.greaterPoints, newBestDist, nextDimension);
		}
		
		if (newBestKDPoint != nullptr) {
			// Check the distance we just got.
			if (shouldReplace(query, currentBestKDPoint, *newBestKDPoint)) {
				currentBestDist = newBestDist;
				currentBestKDPoint = newBestKDPoint;
			}
		}
	}
	
	return currentBestKDPoint;
}

template<int Dim>
bool
etchasketch::KDTree<Dim>::shouldReplace(const KDPoint<Dim> &target,
										const KDPoint<Dim> *currentBest,
										const KDPoint<Dim> &potential) const
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
		return potential < *currentBest;
	}
}

#pragma mark -

template<int Dim>
bool
etchasketch::KDTree<Dim>::contains(const KDPoint<Dim> &query) const
{
	const KDPoint<Dim> *subRoot = root;
	int dimension = 0;
	while (nullptr != subRoot) {
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
bool
etchasketch::KDTree<Dim>::insert(const KDPoint<Dim> &newPoint)
{
	// Make a copy and insert it.
	KDPoint<Dim> *pointCopy = new KDPoint<Dim>(newPoint);
	bool success = insert(pointCopy);
	if (!success) {
		delete pointCopy;
	}
	return success;
}

template<int Dim>
bool
etchasketch::KDTree<Dim>::insert(KDPoint<Dim> *newPoint)
{
	// Safety first.
	if (nullptr == newPoint) {
		return false;
	}
//	// Check if the new point is already in the KDTree.
//	if (contains(*newPoint)) {
//		return false;
//	}
	
	// Temporarily remove the subtrees.
	KDPoint<Dim> *left = newPoint->lesserPoints;
	KDPoint<Dim> *right = newPoint->greaterPoints;
	newPoint->lesserPoints = nullptr;
	newPoint->greaterPoints = nullptr;
	
	if (nullptr != root) {
		// Call the helper.
		if (!insert(*newPoint, *root, 0)) {
			goto warnAndFail;
		}
	} else {
		root = newPoint;
	}
	
	// Insert the subtrees recursively.
	if (nullptr != left) {
		if (!insert(left)) {
			goto warnAndFail;
		}
	}
	if (nullptr != right) {
		if (!insert(right)) {
			goto warnAndFail;
		}
	}
	
	return true; // Success.
	
warnAndFail:
	EASLog("Warning: the KDTree may be in an inconsistent state.");
	return false;
}

template<int Dim>
bool
etchasketch::KDTree<Dim>::insert(KDPoint<Dim> &newPoint,
								 KDPoint<Dim> &subRoot,
								 const int dimension)
{
	// Make sure the new point doesn't already exist.
	if (newPoint == subRoot) {
		return false;
	}
	
	const int nextDimension = (dimension + 1) % Dim;
	// Should we go left or right?
	if (smallerDimVal(newPoint, subRoot, dimension)) {
		// Go left.
		KDPoint<Dim> *lesser = subRoot.lesserPoints;
		if (nullptr != lesser) {
			return insert(newPoint, *lesser, nextDimension);
		} else {
			// Put the new point here.
			subRoot.lesserPoints = &newPoint;
			// TODO: Clear the lesserPoints and greaterPoints of newKDPoint?
			return true;
		}
	} else {
		// Go right.
		KDPoint<Dim> *greater = subRoot.greaterPoints;
		if (nullptr != greater) {
			return insert(newPoint, *greater, nextDimension);
		} else {
			// Put the new point here.
			subRoot.greaterPoints = &newPoint;
			// TODO: Clear the lesserPoints and greaterPoints of newKDPoint?
			return true;
		}
	}
}

template<int Dim>
bool
etchasketch::KDTree<Dim>::remove(const KDPoint<Dim> &target)
{
	KDPoint<Dim> *targetPtr = nullptr;
	// Get the parent point.
	KDPoint<Dim> *parent = const_cast<KDPoint<Dim> *>(getParent(target));
	if (nullptr != parent) {
		// Set the parent's reference to the target point to NULL, effectively
		// removing it from the tree.
		if ((nullptr != parent->lesserPoints) &&
			(*parent->lesserPoints == target))
		{
			targetPtr = parent->lesserPoints;
			parent->lesserPoints = nullptr;
		} else if ((nullptr != parent->greaterPoints) &&
				   (*parent->greaterPoints == target))
		{
			targetPtr = parent->greaterPoints;
			parent->greaterPoints = nullptr;
		}
	} else if ((nullptr != root) && (*root == target)) {
		// The target is the root node.
		targetPtr = root;
		root = nullptr;
	} else {
		// Target is not in the KDTree; return failure.
		return false;
	}
	
	// Reinsert each of its children into the tree.
	if (nullptr != targetPtr) {
		// Reinsert the lesser points.
		insert(targetPtr->lesserPoints);
		targetPtr->lesserPoints = nullptr;
		// Reinsert the greater points.
		insert(targetPtr->greaterPoints);
		targetPtr->greaterPoints = nullptr;
		// Delete the now-removed node.
		delete targetPtr;
	}
	return true; // Success.
}

template<int Dim>
const KDPoint<Dim> *
etchasketch::KDTree<Dim>::getParent(const KDPoint<Dim> &child) const
{
	int dimension = 0;
	const KDPoint<Dim> *subRoot = root;
	while (nullptr != subRoot) {
		// Base case.
		if (subRoot->isLeaf()) {
			// No parent exists.
			return nullptr;
		}
		
		bool lesserIsChild = (nullptr != subRoot->lesserPoints) &&
				(*subRoot->lesserPoints == child);
		bool greaterIsChild = (nullptr != subRoot->greaterPoints) &&
				(*subRoot->greaterPoints == child);
		if (lesserIsChild || greaterIsChild) {
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
etchasketch::KDTree<Dim>::plainPrint(std::ostream &out) const
{
	out << "{\"root\": ";
	// Print all the nodes.
	if (nullptr != root) {
		out << *root;
	} else {
		out << "null";
	}
	out << '}';
}

template<int Dim>
void
etchasketch::KDTree<Dim>::print(std::ostream &out, bool prettyJSON) const
{
	if (!prettyJSON) {
		plainPrint(out);
		return;
	} else {
		// Print all the nodes to a string, then prettify it.
		stringstream ss;
		plainPrint(ss);
		etchasketch::utils::prettyPrintJSON(out, ss);
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
