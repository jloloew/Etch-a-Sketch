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
    motor_t motors[2];
    motor_point_t nibLoc;
	
	/**
	 * Convert points into motor_point_t values.
	 */
	std::vector<motor_point_t> convertToMotorPoints(const std::vector<etchasketch::KDPoint<2>> &points) const;

    double euclideanDistance(const motor_point_t &a,
							 const motor_point_t &b) const;
	
    int moveToPoint(const motor_point_t &pt);
};

#endif /* MotorController_hpp */

