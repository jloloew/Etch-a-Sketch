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

@property etchasketch::KDTree<2> *kdtree;
@property etchasketch::tests::KDTreeTests wrapper;

@end

@implementation KDTreeTests

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
	delete self.kdtree;
	self.kdtree = nullptr;
    [super tearDown];
}

- (void)testConstructorPerformance {
	// Generate random (reproducible) points.
	const size_t n_elems = 10;
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

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
