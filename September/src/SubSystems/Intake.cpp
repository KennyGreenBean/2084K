#include "Intake.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

Intake::Intake(pros::Motor intake) : intake(intake){
    
}

void Intake::brake(){
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

void Intake::telOP(bool intaker, bool outtake){
    if(intaker){
        intake.move_velocity(200);
    }
    else if(outtake){
        intake.move_velocity(-200);
    }
    else{
        intake.move_velocity(0);
    }
}

void Intake::spinIntake(bool direction){
    if(direction){
        intake.move_velocity(200);
    }
    else{
        intake.move_velocity(-200);
    }
}

void Intake::spinIntakeTime(int speed, float time){
    intake.move_velocity(speed);
    pros::delay(time);
    intake.move_velocity(0);
}

void Intake::stop(){
    intake.move_velocity(0);
}