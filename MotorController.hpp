//
//  MotorController.hpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/28/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#ifndef MotorController_hpp
#define MotorController_hpp

#include <stdio.h>
#include "motor.h"
#include "EtchASketch.hpp"

class MotorController {

public:

    /**
     *
     */
    MotorController();

    /**
     *
     */
    void drawOrderedPoints(const std::vector<etchasketch::KDPoint<2>> &points);

private:

    motor_t motorX;
    motor_t motorY;
    motor_point_t nibLoc;

    int moveToPoint(const etchasketch::KDPoint<2> &pt);
};

#endif /* MotorController_hpp */
