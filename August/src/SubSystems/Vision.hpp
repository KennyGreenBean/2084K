#pragma once

#include "api.h"
#include "okapi/api.hpp"

class Vision{
  public:
    Vision(pros::Vision visionSensor);

    int aimbot(bool control, bool toggle);

    pros::Vision visionSensor;
    
};