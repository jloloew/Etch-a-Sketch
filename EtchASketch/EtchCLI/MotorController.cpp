//
//  MotorController.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/28/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "MotorController.hpp"
#include <iostream>

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
    
    if (motor_init(&motorX) || motor_init(&motorY)) {
        fprintf(stderr, "Error creating motor.\n");
        exit(1);
    }
}

void MotorController::drawOrderedPoints(const std::vector<etchasketch::KDPoint<2>> &points)
{
    cout << "DRAWING ORDERED EDGE POINTS" << endl;
    
    // for each point to draw
    for(size_t i = 0; i < points.size(); i++) {

        etchasketch::KDPoint<2> target = points[i];

        int x_dist = target[0] - nibLoc.x;
        int y_dist = target[1] - nibLoc.y;

        // While not at target…
        while (x_dist || y_dist) {
            if (x_dist < 0) {
                motor_prepare_move(&motorX, DIR_CCW);
                x_dist++;
                nibLoc.x--;
            } else if (x_dist > 0) {
                motor_prepare_move(&motorX, DIR_CW);
                x_dist--;
                nibLoc.x++;
            }

            if (y_dist < 0) {
                motor_prepare_move(&motorY, DIR_CCW);
                y_dist++;
                nibLoc.y--;
            } else if (y_dist > 0) {
                motor_prepare_move(&motorY, DIR_CW);
                y_dist--;
                nibLoc.y++;
            }
            
            motor_execute_move(&motorX, 2);
        }
        /*
        etchasketch::KDPoint<2> tgt_pt = points[i];
        float slope = (tgt_pt[1] - nibLoc.y)/(tgt_pt[0] - nibLoc.x);
        
        // while point not reached
        while (nibLoc.x != tgt_pt[0] && nibLoc.y != tgt_pt[1]) {
            
            cout << "x: " << nibLoc.x << ", y: " << nibLoc.y << endl;
            float slope_pt_x = nibLoc.x;
            float slope_pt_y = nibLoc.y;
           
            float ref_pt_x = nibLoc.x;
            float ref_pt_y = nibLoc.y;
            
            if (nibLoc.x < tgt_pt[0]) {
                slope_pt_x++;
                slope_pt_y += slope;
                
                ref_pt_x++;
            }
            if (nibLoc.x > tgt_pt[0]) {
                slope_pt_x--;
                slope_pt_y -= slope;
                
                ref_pt_x--;
            }
            
            if (nibLoc.y < tgt_pt[1]) {
                ref_pt_y++;   
            }
            if (nibLoc.y > tgt_pt[1]) {
                ref_pt_y--;
            }
             
            if (slope_pt_y >= ref_pt_y) {
                // y movement
                if (tgt_pt[1] > nibLoc.y) {
                    motor_move(&motorY, DIR_CW);
                    nibLoc.y++;
                }
                if (tgt_pt[1] < nibLoc.y) {
                    motor_move(&motorY, DIR_CCW);
                    nibLoc.y--;
                }
            } else {
                // x movement
                if (tgt_pt[0] > nibLoc.x) {
                    motor_move(&motorX, DIR_CW);
                    nibLoc.x++;
                }
                if (tgt_pt[0] < nibLoc.x) {
                    motor_move(&motorX, DIR_CCW);
                    nibLoc.x--;
                }
            }
        }
         */
    }
}

int MotorController::moveToPoint(const etchasketch::KDPoint<2> &pt)
{
    return -1;
}
