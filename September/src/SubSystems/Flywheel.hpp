#pragma once

#include "api.h"
// #include "okapi/api.hpp"
// #include "pros/motors.hpp"

class Flywheel{
  public:
    Flywheel(pros::Motor flywheelMotor);
    
    void flywheelTelOp(bool up, bool right, bool left, bool down);
    
  private:
    pros::Motor fMotor;
};