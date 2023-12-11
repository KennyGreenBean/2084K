#pragma once

#include "api.h"
#include "pros/adi.hpp"

class Catapult{
  public:
    Catapult(pros::Motor largeLeft, pros::Motor largeRight, pros::Motor smallLeft, pros::Motor smallRight);
    
    void ptoSwitch(bool currentToggle);
    void brake();
    void telOP(bool pressed);
    void spin();
    void stop();
    
  protected:
    // false = 6m drive, true = 4m drive
    bool pto = false;

    pros::Motor largeLeft;
    pros::Motor largeRight;
    pros::Motor smallLeft;
    pros::Motor smallRight;
};