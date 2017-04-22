//
//  LineSimplifier.cpp
//  EtchASketch
//
//  Created by Justin Loew on 4/4/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "EASUtils+Private.hpp"
#include "LineSimplifier.hpp"

using std::list;
using std::vector;
using etchasketch::KDPoint;

// Forward declares

static float distFromLineSquared(const list<KDPoint<2>>::iterator pointIt,
								 const list<KDPoint<2>>::iterator startIt,
								 const list<KDPoint<2>>::iterator endIt) __attribute__((pure));

/**
 * Find the point on the line that lies the maximum distance from a line drawn
 * through the @c beginning and @c end points.
 *
 * @param beginning An iterator pointing to the first point of the line segment.
 * @param end An iterator pointing to the last point of the line segment.
 * @param outMaxDist An output parameter that contains the distance of the
 * farthest point from the line.
 */
static const list<KDPoint<2>>::iterator
findMaxDistPoint(const list<KDPoint<2>>::iterator &beginning,
				 const list<KDPoint<2>>::iterator &end,
				 float *outMaxDist);

etchasketch::LineSimplifier::LineSimplifier(float epsilon)
: epsilon(epsilon), line(nullptr)
{ }

void
etchasketch::LineSimplifier::simplifyLine(vector<KDPoint<2>> &lineVect)
{
	EASLog("Before simplification: %lu points", lineVect.size());
	
    // Convert to a doubly linked list.
    line = new list<KDPoint<2>>(lineVect.begin(), lineVect.end());
    // Safety checks
    if (line->empty()) {
        return;
    }
    
	// Call out to our implementation.
	douglasPeucker(line->begin(), line->end());
	
	// Convert back to a vector and clean up.
	lineVect.assign(line->begin(), line->end());
	delete this->line;
	this->line = nullptr;
	
	EASLog("After simplification: %lu points", lineVect.size());
}

void
etchasketch::LineSimplifier::douglasPeucker(const list<KDPoint<2>>::iterator &start,
											const list<KDPoint<2>>::iterator &end) const
{
	// Safety first; make sure there's at least one point between the endpoints.
	if ((start == end) || (std::next(start) == end)) {
        return; // Nothing to do; no points to potentially remove.
    }
	
	float maxDist;
    const list<KDPoint<2>>::iterator farPtIter = findMaxDistPoint(start, end, &maxDist);
	// Check if we can eliminate any points.
	if (maxDist > epsilon) {
		// We can't remove the farthest point, but we can split the line at the
		// farthest point and recurse.
		douglasPeucker(start, std::prev(farPtIter));
		douglasPeucker(std::next(farPtIter), end);
	} else {
		// Since the farthest point is within our margin of error, we can safely
		// remove all points between the start and end points.
		line->erase(std::next(start), end);
	}
}

// There must be at least one point between beginning and end.
static
const list<KDPoint<2>>::iterator
findMaxDistPoint(const list<KDPoint<2>>::iterator &beginning,
				 const list<KDPoint<2>>::iterator &end,
				 float *outMaxDist)
{
    list<KDPoint<2>>::iterator curFarthestPoint = beginning;
    float curFarthestDist = -1.0f;
    
    // Check each point between beginning and end.
    list<KDPoint<2>>::iterator curPoint = beginning;
    while (++curPoint != end) {
		float dist = distFromLineSquared(curPoint, beginning, end);
		if (dist > curFarthestDist) {
			curFarthestDist = dist;
			curFarthestPoint = curPoint;
		}
    }
	
	// Return the results.
	*outMaxDist = curFarthestDist;
	return curFarthestPoint;
}

static
float
distFromLineSquared(const list<KDPoint<2>>::iterator pointIt,
					const list<KDPoint<2>>::iterator startIt,
					const list<KDPoint<2>>::iterator endIt)
{
	const KDPoint<2> &point = *pointIt;
	const KDPoint<2> &start = *startIt;
	const KDPoint<2> &end   = *endIt;
	
	KDPointCoordinate proj = (end[1] - start[1]) * point[0] -
							 (end[0] - start[0]) * point[1] +
									      end[0] * start[1] -
							              end[1] * start[0];
	proj = proj * proj;
	KDPointCoordinate dx = end[0] - start[0];
	KDPointCoordinate dy = end[1] - start[1];
	KDPointCoordinate scale = (dx * dx) + (dy * dy);
	return static_cast<float>(proj) / static_cast<float>(scale);
}
