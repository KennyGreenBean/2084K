#include "Catapult.hpp"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.h"
#include "pros/rtos.hpp"

Catapult::Catapult(pros::Motor largeLeft, pros::Motor largeRight, pros::Motor smallLeft, pros::Motor smallRight)
 : largeLeft(largeLeft), largeRight(largeRight), smallLeft(smallLeft), smallRight(smallRight){

}

// controls shooting and reloading of catapult

void Catapult::ptoSwitch(bool currentToggle){
	pto = currentToggle;
}

void Catapult::brake(){
	largeLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	largeRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	smallLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	smallRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

void Catapult::telOP(bool pressed){
	if(pressed){
		if(pto){
			largeLeft.move_velocity(-600);
			largeRight.move_velocity(-600);
			smallLeft.move_velocity(-200);
			smallRight.move_velocity(-200);
		}
		else{
			smallLeft.move_velocity(-200);
			smallRight.move_velocity(-200);
		}
	}
	else{
		largeLeft.move_velocity(0);
		largeRight.move_velocity(0);
		smallLeft.move_velocity(0);
		smallRight.move_velocity(0);
	}
}