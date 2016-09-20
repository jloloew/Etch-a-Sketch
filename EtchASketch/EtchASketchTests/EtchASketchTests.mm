//
//  EtchASketchTests.m
//  EtchASketchTests
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "KDTree.hpp"
#import <vector>

using std::vector;

@interface EtchASketchTests : XCTestCase

@property EtchASketch::KDTree<2> *kdtree;

@end

@implementation EtchASketchTests

- (void)setUp {
    [super setUp];
    // Generate points to put in the new tree.
	vector points;
	points.put(Point<2>(1, 0));
	points.put(Point<2>(3, 4));
	points.put(Point<2>(0, -2));
	self.kdtree = new KDTree<2>(points);
}

- (void)tearDown {
	delete self.kdtree;
	self.kdtree = NULL;
    [super tearDown];
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
