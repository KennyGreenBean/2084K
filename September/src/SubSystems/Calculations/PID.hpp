#pragma once

#include "api.h"
#include "pros/imu.hpp"

class PID{
    public:
    // default constructor
    PID();

    // PID with constants
    PID(double kP, double kI, double kD, double startkI){
        this-> kp = kP;
        this-> ki = kI;
        this-> kd = kD;
        this -> start_i = startkI;
    }

    double kp;
    double ki;
    double kd;
    double start_i;

    // variables
    double output = 0;
    double cur;
    double error = 0;
    double target = 0;
    double prev_error = 0;
    double integral = 0;
    double derivative = 0;
    long time;
    long prev_time;

    void setTarget(double target);

    double getError();

    double compute(double current);
};