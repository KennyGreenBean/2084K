#pragma once

#include "api.h"
#include "okapi/api.hpp"
#include "pros/motors.hpp"

class Intake{
  public:
    Intake(pros::Motor intake);

    void brake();
    void telOP(bool intake, bool outtake);
    void spinIntake(bool direction);
    void spinIntakeTime(int speed, float time);
    void stop();

  private:
    pros::Motor intake;
};