#pragma once

#include "api.h"
#include "okapi/api.hpp"

class Optical{
  public:
    Optical(pros::Optical opticalSensor);

    bool roller(bool blue, bool red, bool bluerepeat, bool redrepeat);

    pros::Optical opticalSensor;
    
};