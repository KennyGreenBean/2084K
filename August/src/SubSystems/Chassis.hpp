#pragma once

#include "api.h"
#include "okapi/api.hpp"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "Intake.hpp"
#include "Calculations/PID.hpp"

class Chassis{
  public:
    Chassis(pros::Motor FrontLeft, pros::Motor FrontRight, pros::Motor MiddleLeft, pros::Motor MiddleRight, pros::Motor BackLeft, pros::Motor BackRight, pros::Imu inertial);

    void ptoSwitch(bool currentToggle);
    void brake();

    void inlineTelOp(int leftY, int rightX);

    void move(int leftSpeed, int rightSpeed);
    void moveDistance(int leftSpeed, int rightSpeed, int distance);
    void stop();
    int getVelocity();
    void resetMotorEncoders();
    double getDriveEncoder();
    void moveSteering(float steering, int speed);
    void moveFowardMotor(double target, int speed);
    void turnInertial(double target);

    // PID Objects
    PID keepStraight = PID(0.5, 0, 0, 0);
    PID forwardController = PID(0.6, 0.01, 0, 50);
    PID turnController = PID(1, 0.02, 0, 20);

    

  protected:
    // false = 6m drive, true = 4m drive
    bool pto = false;

    pros::Motor frontLeft;
    pros::Motor frontRight;
    pros::Motor middleLeft;
    pros::Motor middleRight;
    pros::Motor backLeft;
    pros::Motor backRight;

    int frontleftport = 20;
    int frontrightport = 19;
    int middleleftport = 10;
    int middlerightport = 3;

    pros::Imu inertial;
};