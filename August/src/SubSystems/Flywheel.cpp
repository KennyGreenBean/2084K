#include "Flywheel.hpp"
#include "pros/motors.hpp"

Flywheel::Flywheel(pros::Motor flywheelMotor) : fMotor(flywheelMotor){

}

// spins the flywheel at different speeds depending on which button is pressed
void Flywheel::flywheelTelOp(bool up, bool right, bool left, bool down){
    if(up){
		fMotor.move_voltage(10000);
	}
	else if(right){
		fMotor.move_voltage(12000);
	}
	else if (left){ 
		fMotor.move_voltage(9500);
	}
	else if (down){	
		fMotor.move_voltage(0);
	}
}

