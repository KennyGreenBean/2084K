#include "main.h"
#include "PID.hpp"

using namespace pros;

void PID::setTarget(double target){
    this->target = target;
}

int sgn(double input) {
  if (input > 0)
    return 1;
  else if (input < 0)
    return -1;
  return 0;
}

double PID::getError(){
  return error;
}

double PID::compute(double current){
    error = target - current;
    derivative = error - prev_error;
    
    if (ki != 0) {
        if (fabs(error) < start_i){
            integral += error;
        }
        if (sgn(error) != sgn(prev_error)){
            integral = 0;
        }
    }

    output = (error * kp) + (integral * ki) + (derivative * kd);

    prev_error = error;

  return output;
}