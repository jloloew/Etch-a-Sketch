//
//  MotorController.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/28/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "MotorController.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using std::vector;

MotorController::MotorController()
{
	// Protect against multiple motor controller instances -> singleton.
	static bool singletonGuard = false;
	if (singletonGuard) {
		fprintf(stderr, "Motor controller must be a singleton.\n");
		exit(1);
	}
	singletonGuard = true;
	
	// Initializing motors requires root access.
	motor_initialize();
	
	// Initialize motor nibLoc
	nibLoc.x = 0;
	nibLoc.y = 0;
	
	if (motor_init(&motors[0]) || motor_init(&motors[1])) {
		fprintf(stderr, "Error creating motor.\n");
		exit(1);
	}
}

void
MotorController::drawOrderedPoints(const std::vector<etchasketch::KDPoint<2>> &points)
{
	// Scale the points to fit the motor's drawable resolution.
	const vector<motor_point_t> motorPoints = convertToMotorPoints(points);
	
	// for each point to draw
	for (auto it = motorPoints.begin(); it != motorPoints.end(); ++it) {
		motor_point_t goal_pt = *it;
		motor_point_t start_pt = nibLoc;
		motor_point_t current_pt = start_pt;
		
		// while not at goal_pt
		while ((current_pt.x != goal_pt.x) || (current_pt.y != goal_pt.y)) {
			// enumerate possible next coordinates
			motor_point_t n, e, s, w;
			n = current_pt; e = current_pt; s = current_pt; w = current_pt;
			
			if (current_pt.y > 0.0f) {
				n.y -= 1.0f;
			}
			if (current_pt.x > 0.0f) {
				e.x -= 1.0f;
			}
			if (current_pt.y < motor_max_loc[1]) {
				s.y += 1.0f;
			}
			if (current_pt.x < motor_max_loc[0]) {
				w.x += 1.0f;
			}
			
			// calculate distance from possible choices to goal_pt
			double curr_dist, n_dist, e_dist, s_dist, w_dist;
			curr_dist = euclideanDistance(current_pt, goal_pt);
			n_dist = euclideanDistance(n, goal_pt);
			e_dist = euclideanDistance(e, goal_pt);
			s_dist = euclideanDistance(s, goal_pt);
			w_dist = euclideanDistance(w, goal_pt);
			
			// generate chosen_pt that minimizes distance to goal_pt
			motor_point_t chosen_pt = current_pt;
			
			// X
			if (e_dist < curr_dist || w_dist < curr_dist) {
				if (e_dist < w_dist) {
					chosen_pt.x = e.x;
				} else {
					chosen_pt.x = w.x;
				}
			}
			
			// Y
			if (n_dist < curr_dist || s_dist < curr_dist) {
				if (n_dist < s_dist) {
					chosen_pt.y = n.y;
				} else {
					chosen_pt.y = s.y;
				}
			}
			
			// move to chosen point
			moveToPoint(chosen_pt);
			current_pt = chosen_pt;
			
			// debugging
			// cout << "n x: " << n[0] << ", y: " << n[1] << endl;
			// cout << "     n_dist:" << n_dist << endl;
			// cout << "s x: " << s[0] << ", y: " << s[1] << endl;
			// cout << "     s_dist:" << s_dist << endl;
			// cout << "e x: " << e[0] << ", y: " << e[1] << endl;
			// cout << "     e_dist:" << e_dist << endl;
			// cout << "w x: " << w[0] << ", y: " << w[1] << endl;
			// cout << "     w_dist:" << w_dist << endl;
			//
			// cout << "chosen_pt x: " << chosen_pt[0] << ", y: " << chosen_pt[1] << endl;
			// cout << "goal_pt x: " << goal_pt[0] << ", y: " << goal_pt[1] << endl;
			// cout << "----------" << endl;
			// cout << "current_pt x: " << current_pt[0] << ", y: " << current_pt[1] << endl;
			// cout << "----------" << endl << endl;
			//
			// sleep(1);
			
		} // while not at goal_pt
	} // for each point
}

vector<motor_point_t>
MotorController::convertToMotorPoints(const vector<etchasketch::KDPoint<2>> &points) const
{
	vector<motor_point_t> motorPoints = vector<motor_point_t>();
	motorPoints.reserve(points.size());
	// Convert each KDPoint to a motor_point_t.
	for (auto it = points.begin(); it != points.end(); ++it) {
		motor_point_t mpt = {
			.x = static_cast<float>((*it)[0]),
			.y = static_cast<float>((*it)[1])
		};
		motorPoints.push_back(mpt);
	}
	return motorPoints;
}

double
MotorController::euclideanDistance(const motor_point_t &a,
                                   const motor_point_t &b) const
{
	const double x = a.x - b.x;
	const double y = a.y - b.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}

int
MotorController::moveToPoint(const motor_point_t &pt)
{
    const float x_dist = pt.x - nibLoc.x;
    const float y_dist = pt.y - nibLoc.y;
    
    // Control motors
	if (x_dist > 0.0f) {
		motor_prepare_move(&motors[0], DIR_CW);
	} else if (x_dist < 0.0f) {
		motor_prepare_move(&motors[0], DIR_CCW);
	}
    
	if (y_dist > 0.0f) {
		motor_prepare_move(&motors[1], DIR_CCW);
	} else if (y_dist < 0.0f) {
		motor_prepare_move(&motors[1], DIR_CW);
	}
    
	motor_execute_move(motors, 2);
    
    cout << "nibLoc: x: " << nibLoc.x << ", y: " << nibLoc.y << endl;
    
    // Update nibLoc coordinates
    nibLoc.x = pt.x;
    nibLoc.y = pt.y;
    
    return 0;
}

