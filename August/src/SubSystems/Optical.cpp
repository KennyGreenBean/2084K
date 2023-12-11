#include "Optical.hpp"

bool Optical::roller(bool color1, bool color2, bool blueRepeat, bool redRepeat){
    if(color1 || blueRepeat){
        if(opticalSensor.get_hue() > 140 || blueRepeat){
            return true;
        }
    }
    else if(color2 || redRepeat){
        if(opticalSensor.get_hue() < 60 || redRepeat){
            return false;
        }
    }
    else {
        blueRepeat = false;
        redRepeat = false;
        return true;
    }
    return true;
}