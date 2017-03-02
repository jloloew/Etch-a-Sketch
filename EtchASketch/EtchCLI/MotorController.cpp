//
//  MotorController.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/28/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include "MotorController.hpp"

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


    if (motor_init(&motorX) || motor_init(&motorY)) {
        fprintf(stderr, "Error creating motor.\n");
        exit(1);
    }
}

void MotorController::drawOrderedPoints(const std::vector<etchasketch::KDPoint<2>> &points)
{

}

int MotorController::moveToPoint(const etchasketch::KDPoint<2> &pt)
{
    return -1;
}
