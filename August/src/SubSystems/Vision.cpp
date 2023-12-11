#include "Vision.hpp"

int Vision::aimbot(bool control, bool toggle){
    toggle = true;
    pros::vision_object_s_t rtn = visionSensor.get_by_sig(0, 1);
    int xCoord = rtn.x_middle_coord;
    if(control){
        toggle = false;
    }
    else if(xCoord > 10){
        return 1;
    }
    else if(xCoord < 7){
        return 2;
    }
    else{
        return 3;
    } 
    return 0;
}