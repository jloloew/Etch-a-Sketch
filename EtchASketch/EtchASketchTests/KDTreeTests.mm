//
//  KDTreeTests.m
//  EtchASketch
//
//  Created by Justin Loew on 10/10/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "KDTree.hpp"
#import <vector>

using std::vector;
using etchasketch::KDTree;
using etchasketch::KDPoint;

class etchasketch::tests::KDTreeTests {
public:
	bool wrapSmallerDimVal(KDTree<2> *kdtree,
						   const KDPoint<2> &first,
						   const KDPoint<2> &second,
						   const int curDim)
	{
		return kdtree->smallerDimVal(first, second, curDim);
	}
};

@interface KDTreeTests : XCTestCase

@property (nonatomic) etchasketch::KDTree<2> *kdtree;
@property etchasketch::tests::KDTreeTests wrapper;

@end

@implementation KDTreeTests

- (void)setKdtree:(etchasketch::KDTree<2> *)kdtree {
	// Gotta manage memory, even if it's just a test suite.
	delete _kdtree;
	_kdtree = kdtree;
}

- (void)setUp {
    [super setUp];
	// Generate points to put in the new tree.
	vector<KDPoint<2>> points;
	points.push_back(KDPoint<2>(1, 0));
	points.push_back(KDPoint<2>(3, 4));
	points.push_back(KDPoint<2>(0, -2));
	self.kdtree = new KDTree<2>(points);
	self.wrapper = etchasketch::tests::KDTreeTests();
}

- (void)tearDown {
	self.kdtree = nullptr; // Delete the KDTree.
    [super tearDown];
}

- (void)testConstructorPerformance {
	// Generate random (reproducible) points.
	const size_t n_elems = 1000000;
	srand(0x8BADF00D);
	unordered_set<KDPoint<2>> *points = new unordered_set<KDPoint<2>>(n_elems);
	for (size_t i = 0; i < n_elems; i++) {
		KDPoint<2> pt(rand(), rand());
		points->insert(pt);
	}
	// Create the KDTree.
	[self measureBlock:^{
		KDTree<2> kd(*points);
	}];
	delete points;
}

- (void)testSmallerDimVal {
	KDPoint<2> neg(-1, -1), both(-1, 1), pos(1, 1);
	// Test expected results.
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, neg, pos, 0));
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, neg, pos, 1));
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, neg, both, 0)); // True due to tiebreaking < operator.
	XCTAssertFalse(self.wrapper.wrapSmallerDimVal(self.kdtree, both, neg, 0));
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, neg, both, 1));
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, both, pos, 0));
	XCTAssertTrue(self.wrapper.wrapSmallerDimVal(self.kdtree, both, pos, 1)); // True due to tiebreaking < operator.
	XCTAssertFalse(self.wrapper.wrapSmallerDimVal(self.kdtree, pos, both, 1));
	// Test invalid dimensions.
	try {
		self.wrapper.wrapSmallerDimVal(self.kdtree, neg, neg, -1);
		XCTFail(@"Should've thrown.");
	} catch (const std::out_of_range &) {
		// Correct; carry on.
	} catch (...) {
		XCTFail(@"Unknown exception");
	}
	try {
		self.wrapper.wrapSmallerDimVal(self.kdtree, neg, neg, 2);
		XCTFail(@"Should've thrown.");
	} catch (const std::out_of_range &) {
		// Correct; carry on.
	} catch (...) {
		XCTFail(@"Unknown exception");
	}
}

- (void)testFindNearestNeighbor {
	// Generate points to put in the new tree.
	vector<KDPoint<2>> points;
	const KDPoint<2> pt_0_0(0, 0), pt_1_1(1, 1), pt_2_2(2, 2);
	points.push_back(pt_0_0);
	points.push_back(pt_1_1);
	points.push_back(pt_2_2);
	KDTree<2> kdtree(points);
	cout << kdtree << endl;
	const KDPoint<2> *nn;
	// Test normal find nearest neighbor
	KDTree<2> kdtree1;
	kdtree1.insert(pt_1_1);
	kdtree1.insert(pt_2_2);
	nn = kdtree1.findNearestNeighbor(pt_0_0);
	XCTAssert(nullptr != nn);
	XCTAssert(pt_1_1 == *nn);
	// Test tie for nearest neighbor.
	KDTree<2> kdtree2;
	kdtree2.insert(pt_0_0);
	kdtree2.insert(pt_2_2);
	nn = kdtree2.findNearestNeighbor(pt_1_1);
	XCTAssert(nullptr != nn);
	XCTAssert(pt_0_0 == *nn);
	// Test nn of point that's in the tree.
	KDTree<2> kdtree3;
	kdtree3.insert(pt_0_0);
	kdtree3.insert(pt_1_1);
	kdtree3.insert(pt_2_2);
	nn = kdtree3.findNearestNeighbor(pt_2_2);
	XCTAssert(nullptr != nn);
	XCTAssertEqual(pt_2_2, *nn);
	
	// Test nn of empty tree.
	KDTree<2> kdtree4;
	nn = kdtree4.findNearestNeighbor(pt_0_0);
	XCTAssert(nullptr == nn);
	// Test nn of tree with a single point.
	KDTree<2> kdtree5;
	kdtree5.insert(pt_1_1);
	// Should return the only point in the tree.
	nn = kdtree5.findNearestNeighbor(pt_2_2);
	XCTAssert(nullptr != nn);
	XCTAssertEqual(pt_1_1, *nn);
	// Test finding NN of the only point in the tree.
	nn = kdtree5.findNearestNeighbor(pt_1_1);
	XCTAssert(nullptr != nn);
	XCTAssert(pt_1_1 == *nn);
}

@end
