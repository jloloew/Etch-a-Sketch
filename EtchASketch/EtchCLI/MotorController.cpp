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

MotorController::MotorController()
{
    // Protect against multiple motor controller instances -> singleton.
    static char singletonGuard = 0;
    if (singletonGuard != 0) {
        fprintf(stderr, "Motor controller must be a singleton.\n");
        exit(1);
    }
    singletonGuard = 1;

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

void MotorController::drawOrderedPoints(const std::vector<etchasketch::KDPoint<2>> &points)
{
  // for each point to draw
  for(size_t i = 0; i < points.size(); i++) {
    etchasketch::KDPoint<2> goal_pt = points[i];
    etchasketch::KDPoint<2> start_pt = etchasketch::KDPoint<2>(nibLoc.x, nibLoc.y);
    etchasketch::KDPoint<2> current_pt = start_pt;

    // while not at goal_pt
    while((current_pt[0] != goal_pt[0]) || (current_pt[1] != goal_pt[1])) {

      // enumerate possible next coordinates
      etchasketch::KDPoint<2> n, e, s, w;
      n = current_pt; e = current_pt; s = current_pt; w = current_pt;

      if (current_pt[1] > 0) n[1]--;
      if (current_pt[0] > 0) e[0]--;
      if (current_pt[1] < EAS_BOARD_HEIGHT) s[1]++;
      if (current_pt[0] < EAS_BOARD_WIDTH) w[0]++;

      // calculate distance from possible choices to goal_pt
      double curr_dist, n_dist, e_dist, s_dist, w_dist;
      curr_dist = euclidean_distance(current_pt, goal_pt);
      n_dist = euclidean_distance(n, goal_pt);
      e_dist = euclidean_distance(e, goal_pt);
      s_dist = euclidean_distance(s, goal_pt);
      w_dist = euclidean_distance(w, goal_pt);

      // generate chosen_pt that minimizes distance to goal_pt
      etchasketch::KDPoint<2> chosen_pt = current_pt;

      // X
      if (e_dist < curr_dist || w_dist < curr_dist) {
        if (e_dist < w_dist)
          chosen_pt[0] = e[0];
        else
          chosen_pt[0] = w[0];
      }

      // Y
      if (n_dist < curr_dist || s_dist < curr_dist) {
        if (n_dist < s_dist)
          chosen_pt[1] = n[1];
        else
          chosen_pt[1] = s[1];
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

double MotorController::euclidean_distance(const etchasketch::KDPoint<2> &a,
                                           const etchasketch::KDPoint<2> &b)
{
  double x = a[0] - b[0];
  double y = a[1] - b[1];
  return sqrt(pow(x, 2) + pow(y, 2));
}

int MotorController::moveToPoint(const etchasketch::KDPoint<2> &pt)
{
 
    int x_dist = pt[0] - nibLoc.x;
    int y_dist = pt[1] - nibLoc.y;
    
    // control motors!
    for (size_t i = 0; i < 100; i++) {
        if (x_dist > 0) {
            motor_prepare_move(&motors[0], DIR_CW);
        } else if (x_dist < 0) {
            motor_prepare_move(&motors[0], DIR_CCW);
        }
        
        if (y_dist > 0) {
            motor_prepare_move(&motors[1], DIR_CCW);
        } else if (y_dist < 0) {
            motor_prepare_move(&motors[1], DIR_CW);
        }

        motor_execute_move(motors, 2);
    }
    
    cout << "x: " << nibLoc.x << ", y: " << nibLoc.y << endl;
    
    // update nibLoc coordinates
    nibLoc.x = pt[0];
    nibLoc.y = pt[1];
    
    return 0;
}
