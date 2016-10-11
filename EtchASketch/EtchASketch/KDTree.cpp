//
//  KDTree.cpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

//#include "KDTree.hpp"
#include <algorithm>

using std::unordered_set;
using std::vector;


#pragma mark - Constructors

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const unordered_set<etchasketch::KDPoint<Dim> *> &newKDPoints)
{
	if (newKDPoints.empty()) {
		root = nullptr;
		return;
	}
	
	buildTree(newKDPoints);
}

template<int Dim>
etchasketch::KDTree<Dim>::KDTree(const vector<KDPoint<Dim> *> &newKDPoints)
: KDTree(unordered_set<etchasketch::KDPoint<Dim> *>(newKDPoints.begin(),
													newKDPoints.end()))
{ }

template<int Dim>
void
etchasketch::KDTree<Dim>::buildTree(const unordered_set<KDPoint<Dim> *> &points)
{
	// TODO: Would sorting the points help speed up the build?
	// Just insert each point.
	for (auto pointIter = points.begin(); pointIter != points.end(); pointIter++) {
		KDPoint<2> *point = *pointIter;
		if (nullptr != point) {
			this->insert(*point);
		}
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
etchasketch::KDTree<Dim>::smallerDimVal(const etchasketch::KDPoint<Dim> &first,
										const etchasketch::KDPoint<Dim> &second,
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
const etchasketch::KDPoint<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const etchasketch::KDPoint<Dim> &query) const
{
	double currentBestDist = 1.0 / 0.0; // infinity
	return findNearestNeighbor(query, root, currentBestDist, 0);
}

template<int Dim>
const etchasketch::KDPoint<Dim> *
etchasketch::KDTree<Dim>::findNearestNeighbor(const etchasketch::KDPoint<Dim> &query,
											  const etchasketch::KDPoint<Dim> *subroot,
											  double &currentBestDist, // inout
											  const int dimension) const
{
	// Base cases.
	if (subroot == nullptr) {
		return nullptr;
	}
	const etchasketch::KDPoint<Dim> subRoot = *subroot;
	if (subRoot.isLeaf()) {
		currentBestDist = query.distanceTo(subRoot);
		return subroot;
	}
	
	const int nextDimension = (dimension + 1) % Dim;
	const double myDistance = query.distanceTo(subRoot);
	const etchasketch::KDPoint<Dim> *currentBestKDPoint = nullptr;
	
	// Recurse to find the nearest neighbor in a subtree.
	if (smallerDimVal(query, subRoot, dimension)) {
		// Go left.
		currentBestKDPoint = findNearestNeighbor(query, subRoot.lesserKDPoints, currentBestDist, nextDimension);
	} else {
		// Go right.
		currentBestKDPoint = findNearestNeighbor(query, subRoot.greaterKDPoints, currentBestDist, nextDimension);
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
		const etchasketch::KDPoint<Dim> *newBestKDPoint;
		if (smallerDimVal(subRoot, query, dimension)) {
			// Go left.
			newBestKDPoint = findNearestNeighbor(query, subRoot.lesserKDPoints, newBestDist, nextDimension);
		} else {
			// Go right.
			newBestKDPoint = findNearestNeighbor(query, subRoot.greaterKDPoints, newBestDist, nextDimension);
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
	const KDPoint<Dim> *subRoot = *root;
	int dimension = 0;
	while (subRoot != nullptr) {
		// Base case.
		if (*subRoot == query) {
			return true;
		}
		
		if (smallerDimVal(query, *subRoot, dimension)) {
			// Go left.
			subRoot = subRoot->lesserKDPoints;
		} else {
			// Go right.
			subRoot = subRoot->greaterKDPoints;
		}
		dimension = (dimension + 1) % Dim;
	}
	return false;
}

#pragma mark - Tree modification

template<int Dim>
void
etchasketch::KDTree<Dim>::insert(KDPoint<Dim> &newKDPoint)
{
	// Temporarily remove the subtrees.
	KDPoint<Dim> *left = newKDPoint.lesserKDPoints;
	KDPoint<Dim> *right = newKDPoint.greaterKDPoints;
	newKDPoint.lesserKDPoints = nullptr;
	newKDPoint.greaterKDPoints = nullptr;
	
	if (root != nullptr) {
		// Call the helper.
		insert(newKDPoint, *root, 0);
	} else {
		root = &newKDPoint;
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
etchasketch::KDTree<Dim>::insert(KDPoint<Dim> &newKDPoint,
								 KDPoint<Dim> &subRoot,
								 const int dimension)
{
	const int nextDimension = (dimension + 1) % Dim;
	// Should we go left or right?
	if (smallerDimVal(newKDPoint, subRoot, dimension)) {
		// Go left.
		KDPoint<Dim> *lesser = subRoot.lesserKDPoints;
		if (lesser != nullptr) {
			insert(newKDPoint, *lesser, nextDimension);
		} else {
			// Put the new point here.
			subRoot.lesserKDPoints = &newKDPoint;
			// TODO: Clear the lesserKDPoints and greaterKDPoints of newKDPoint?
		}
	} else {
		// Go right.
		KDPoint<Dim> *greater = subRoot.greaterKDPoints;
		if (greater != nullptr) {
			insert(newKDPoint, *greater, nextDimension);
		} else {
			// Put the new point here.
			subRoot.greaterKDPoints = &newKDPoint;
			// TODO: Clear the lesserKDPoints and greaterKDPoints of newKDPoint?
		}
	}
}

template<int Dim>
void
etchasketch::KDTree<Dim>::remove(KDPoint<Dim> *&targetKDPoint)
{
	// Safety first.
	if (targetKDPoint == nullptr) {
		return;
	}
	
	// Get the parent point.
	KDPoint<Dim> *parent = const_cast<KDPoint<Dim> *>(getParent(*targetKDPoint));
	if (parent != nullptr) {
		// Set the parent's reference to the target point to NULL, effectively
		// removing it from the tree.
		if (parent->lesserKDPoints == targetKDPoint) {
			targetKDPoint = parent->lesserKDPoints;
			parent->lesserKDPoints = nullptr;
		} else if (parent->greaterKDPoints == targetKDPoint) {
			targetKDPoint = parent->greaterKDPoints;
			parent->greaterKDPoints = nullptr;
		}
	} else if (targetKDPoint == root) {
		// The target is the root node.
		targetKDPoint = root;
		root = nullptr;
	}
	
	// Reinsert each of its children into the tree.
	if (targetKDPoint->lesserKDPoints != nullptr) {
		insert(*targetKDPoint->lesserKDPoints);
		targetKDPoint->lesserKDPoints = nullptr;
	}
	if (targetKDPoint->greaterKDPoints != nullptr) {
		insert(*targetKDPoint->greaterKDPoints);
		targetKDPoint->lesserKDPoints = nullptr;
	}
	
	// Delete the now-removed node.
	delete targetKDPoint;
	targetKDPoint = nullptr;
}

template<int Dim>
const etchasketch::KDPoint<Dim> *
etchasketch::KDTree<Dim>::getParent(const etchasketch::KDPoint<Dim> &child) const
{
	int dimension = 0;
	const KDPoint<Dim> *subRoot = root;
	while (subRoot != nullptr) {
		// Base case.
		if (subRoot->isLeaf()) {
			// No parent exists.
			return nullptr;
		}
		if ((*subRoot->lesserKDPoints == child) || (*subRoot->greaterKDPoints == child)) {
			return subRoot;
		}
		
		if (smallerDimVal(child, *subRoot, dimension)) {
			// Go left.
			subRoot = subRoot->lesserKDPoints;
		} else {
			// Go right.
			subRoot = subRoot->greaterKDPoints;
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
