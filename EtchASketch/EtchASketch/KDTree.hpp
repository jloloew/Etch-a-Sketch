//
//  KDTree.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef KDTree_hpp
#define KDTree_hpp

#include <iostream>
#include <unordered_set>
#include <vector>
#include "KDPoint.hpp"

namespace etchasketch {
	
	namespace tests {
		class KDTreeTests;
	}
	
	/// An implementation of a k-D tree.
	template<int Dim>
	class KDTree
	{
		friend class tests::KDTreeTests;
		
	public:
		/**
		 * Constructs a KDTree from a vector of KDPoints, each having dimension
		 * Dim.
		 *
		 * You may assume the vector has at least one KDPoint in it. It should
		 * build the tree using recursive helper functions.
		 *
		 * Since we know the size of the KDTree at construction, we can
		 * represent the tree as a linear structure. For testing, we
		 * **require** the following:
		 *
		 * 1.  The root of a subtree in the KDTree containing array indicies
		 *     \f$\{a, a+1, a+2, ..., b-1, b\}\f$ **must** be in cell
		 *     \f$\left\lfloor\frac{a+b}{2}\right\rfloor\f$.
		 *
		 *     This means that, in particular, that the root of the KDTree is
		 *     positioned at index
		 *     \f$\left\lfloor\frac{0+(n-1)}{2}\right\rfloor\f$.
		 *
		 *     As you build the tree, your recursive tree building function
		 *     calls will refer to points on the left of this index, and to
		 *     points on the right of this index.
		 * 2.  The median node of n nodes is calculated as the cell
		 *     \f$\left\lfloor\frac{n-1}{2}\right\rfloor\f$. That is, the
		 *     middle node is selected if there are an odd number, and the
		 *     item before the middle if there are an even number. **If there
		 *     are ties (two points have equal value along a dimension), they
		 *     must be decided using KDPoint::operator<().** Although this is
		 *     arbitrary and doesn't affect the functionality of the KDTree,
		 *     it is required to be able to grade your code.
		 *
		 * KD-trees are created recursively; at any stage of the construction,
		 * the median value in the current dimension is selected. Then, all
		 * the elements in the current subtree are divided up into elements
		 * which are less than the median, or greater than the median, and
		 * then the subtrees are created recursively. The children pick the
		 * median in the next dimension, and repeat until the entire set of
		 * inputs has been processed. Successive levels of the tree split on
		 * increasing dimensions, modulo the total number: a 3D tree will
		 * have levels split by dimension 0, 1, 2, 0, 1, 2, etc.
		 *
		 * @param newPoints The vector of points to build your KDTree off of.
		 */
		KDTree(const std::vector<etchasketch::KDPoint<Dim>> &newPoints);
		
		KDTree(const std::unordered_set<etchasketch::KDPoint<Dim>> &newPoints);
		
		KDTree();
		
		virtual ~KDTree(void);
		
		/**
		 * Finds the closest point to the parameter point in the KDTree.
		 *
		 * This function takes a reference to a template parameter KDPoint and
		 * returns the KDPoint closest to it in the tree. We are defining
		 * closest here to be the minimum Euclidean distance between elements.
		 * Again, **if there are ties (this time in distance), they must be
		 * decided using KDPoint::operator<().** Recall that an RGBAPixel is
		 * defined by three components: red, green, and blue.
		 *
		 * The findNearestNeighbor() search is done in two steps: a search to
		 * find the smallest hyperrectangle that contains the target element,
		 * and then a back traversal to see if any other hyperrectangle could
		 * contain a closer point, which may be a point with smaller distance
		 * or a point with equal distance, but a "smaller" point (as defined
		 * by operator< in the point class). In the first step, you must
		 * recursively traverse down the tree, at each level choosing the
		 * subtree which represents the region containing the search element
		 * (another place to save some duplicate code?). When you reach the
		 * lowest bounding hyperrectangle, then the corresponding node is
		 * effectively the "current best" neighbor.
		 *
		 * However, it may be the case that a better match exists outside of
		 * the containing hyperrectangle. At then end of first step of the
		 * search, we start traversing back up the kdtree to the parent node.
		 * The current best distance defines a radius which contains the
		 * nearest neighbor. During the back-traversal (i.e., stepping out of
		 * the recursive calls), you must first check if the distance to the
		 * parent node is less than the current radius. If so, then that
		 * distance now defines the radius, and we replace the "current best"
		 * match. Next, it is necessary to check to see if the current
		 * splitting plane's distance from search node is within the current
		 * radius. If so, then the opposite subtree could contain a closer
		 * node, and must also be searched recursively.
		 *
		 * During the back-traversal, it is important to only check the
		 * subtrees that are within the current radius, or else the efficiency
		 * of the kdtree is lost. If the distance from the search node to the
		 * splitting plane is greater than the current radius, then there
		 * cannot possibly be a better nearest neighbor in the subtree, so the
		 * subtree can be skipped entirely.
		 *
		 * @c findNearestNeighbor should only be called on a valid kd-tree.
		 *
		 * @param query The point we wish to find the closest neighbor to in the
		 * tree.
		 * @return A copy of the closest point to @c query in the KDTree.
		 */
		etchasketch::KDPoint<Dim> *
		findNearestNeighbor(const etchasketch::KDPoint<Dim> &query) const;
		
		/**
		 * Check whether the KD tree contains a given point.
		 * @param query The point to see if it's in the KD tree.
		 * @return @c true if @c query is present in the KD tree, @c false
		 * otherwise.
		 */
		bool contains(const etchasketch::KDPoint<Dim> &query) const;
		
		/**
		 * Insert a new point.
		 * @param newPoint The new point to be added to the KD tree.
		 * @return @c true on success, or @c false if @c newPoint already exists.
		 */
		bool insert(const etchasketch::KDPoint<Dim> &newPoint);
		
		/**
		 * Remove a given node from the KD tree, then delete the node. The
		 * target's subtrees are retained in the KD tree.
		 * @param target The node to be removed and deleted.
		 * @return @c true if @c target was successfully removed, @c false
		 *  otherwise.
		 */
		bool remove(const etchasketch::KDPoint<Dim> &target);
		
		/// Print the KD tree, one node at a time.
		void print(std::ostream &out = std::cout, bool prettyJSON = true) const;
		
		/// Because std::cout isn't available from within lldb.
		void debugPrint() const
			{ print(); }
		
	private:
		/// This is the root node of our KDTree representation.
		etchasketch::KDPoint<Dim> *root;
		
		/// Helper function for the KDTree constructor.
		void
		buildTree(const std::unordered_set<etchasketch::KDPoint<Dim>> &points);
		
		/**
		 * Delete a subtree.
		 * @param subRoot inout The root of the subtree to be deleted.
		 */
		void deleteSubtree(etchasketch::KDPoint<Dim> *subRoot);
		
		/**
		 * Like regular insert, but takes a pointer for a pre-copied point.
		 * @return @c true on success, or @c false on failure.
		 */
		bool insert(etchasketch::KDPoint<Dim> *newPoint);
		
		/**
		 * Helper function for insert.
		 * @return @c true on success, or @c false on failure.
		 */
		bool insert(etchasketch::KDPoint<Dim> &newPoint,
					etchasketch::KDPoint<Dim> &subRoot,
					const int dimension);
		
		const etchasketch::KDPoint<Dim> *
		getParent(const etchasketch::KDPoint<Dim> &child) const;
		
		const etchasketch::KDPoint<Dim> *
		findNearestNeighbor(const etchasketch::KDPoint<Dim> &query,
							const etchasketch::KDPoint<Dim> *subroot,
							double &currentBestDist, // inout
							const int dimension) const;
		
		/**
		 * Determines if KDPoint a is smaller than KDPoint b in a given
		 * dimension d. If there is a tie, break it with KDPoint::operator<().
		 *
		 * For example:
		 *
		 *     KDPoint<3> a(1, 2, 3);
		 *     KDPoint<3> b(3, 2, 1);
		 *     cout << smallerDimVal(a, b, 0) << endl; // should print true
		 *     cout << smallerDimVal(a, b, 2) << endl; // should print false
		 *     cout << smallerDimVal(a, b, 1) << endl; // based on operator<, this should be true
		 *
		 * @param first KDPoint to compare.
		 * @param second Second point to compare.
		 * @param curDim Dimension these points are being compared in.
		 * @return A boolean value indicating whether the first KDPoint is
		 *  smaller than the second KDPoint in the curDim dimension.
		 */
		bool smallerDimVal(const etchasketch::KDPoint<Dim> &first,
						   const etchasketch::KDPoint<Dim> &second,
						   const int curDim) const;
		
		/**
		 * Determines if a KDPoint is closer to the target KDPoint than another
		 * reference KDPoint. Takes three points: target, currentBest, and
		 * potential, and returns whether or not potential is closer to
		 * target than currentBest.
		 *
		 * We are using Euclidean distance to compare k-dimensional
		 * points: \f$\sqrt{(p_1-q_1)^2+(p_2-q_2)^2+\ldots+(p_n-q_n)^2} =
		 * \sqrt{\sum_{i=1}^n (p_i-q_i)^2}\f$. Note, however, that
		 * **minimizing distance is the same as minimizing squared
		 * distance**, so you can **avoid invoking the square root** and
		 * just compare squared distances in your code.
		 *
		 * For example:
		 *
		 *     KDPoint<3> target(1, 3, 5);
		 *     KDPoint<3> currentBest1(1, 3, 2);
		 *     KDPoint<3> possibleBest1(2, 4, 4);
		 *     KDPoint<3> currentBest2(1, 3, 6);
		 *     KDPoint<3> possibleBest2(2, 4, 4);
		 *     KDPoint<3> currentBest3(0, 2, 4);
		 *     KDPoint<3> possibleBest3(2, 4, 6);
		 *     cout << shouldReplace(target, currentBest1, possibleBest1) << endl; // true
		 *     cout << shouldReplace(target, currentBest2, possibleBest2) << endl; // false
		 *     cout << shouldReplace(target, currentBest3, possibleBest3) << endl;
		 *      // based on operator<, this should be false
		 *
		 * @param target The KDPoint we want to be close to.
		 * @param currentBest The KDPoint that is currently our closest KDPoint
		 *    to target.
		 * @param potential Our KDPoint that is a candidate to replace
		 *    currentBest (that is, it may be closer to target than
		 *    currentBest).
		 * @return A boolean value indicating whether potential is closer
		 *  to target than currentBest. Ties should be broken with
		 *  KDPoint::operator<().
		 */
		bool shouldReplace(const etchasketch::KDPoint<Dim> &target,
						   const etchasketch::KDPoint<Dim> *currentBest,
						   const etchasketch::KDPoint<Dim> &potential) const;
		
		void plainPrint(std::ostream &out) const;
	};
	
	template<int Dim>
	std::ostream & operator<<(std::ostream &out, const KDTree<Dim> &kdtree);
	
}

#include "KDTree.cpp"

#endif /* KDTree_hpp */
