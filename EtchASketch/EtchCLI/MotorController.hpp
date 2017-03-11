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


#define EAS_BOARD_WIDTH 1000
#define EAS_BOARD_HEIGHT 1000

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

    motor_t motors[2];
    motor_point_t nibLoc;

    double euclidean_distance(const etchasketch::KDPoint<2> &a,
                              const etchasketch::KDPoint<2> &b);
    int moveToPoint(const etchasketch::KDPoint<2> &pt);
};

#endif /* MotorController_hpp */
