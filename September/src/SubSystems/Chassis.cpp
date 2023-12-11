#include "Chassis.hpp"
#include "okapi/impl/util/timer.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <vector> 	
#include <cmath>
#include "api.h"
#include "okapi/api.hpp"
#include "time.h"

using namespace okapi;

Chassis::Chassis(pros::Motor FrontLeft, pros::Motor FrontRight, pros::Motor MiddleLeft, pros::Motor MiddleRight, pros::Motor BackLeft, pros::Motor BackRight, pros::Imu inertial) : 
	frontLeft(FrontLeft), frontRight(FrontRight), middleLeft(MiddleLeft), middleRight(MiddleRight), backLeft(BackLeft), backRight(BackRight), inertial(inertial){ 
}

void Chassis::ptoSwitch(bool currentToggle){
    pto = currentToggle;
}

// sets all the motor brake modes to brake, so our robot will stop immediately when we tell it to stop
void Chassis::brake(){
    frontRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	frontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    middleLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    middleRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	backLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	backRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

// Gets the values from the joystick to tell the motors how fast to move
void Chassis::inlineTelOp(int leftY, int rightX){// 127, -127
    float forward = 4.7244 * leftY;
    float turn = 4.7244 * rightX;//4.7244 * rightX
    
    // if(pto){
    //     frontLeft.move_velocity(forward + turn);
    //     middleLeft.move_velocity(forward + turn);	
    //     frontRight.move_velocity(forward - turn);
    //     middleRight.move_velocity(forward - turn);
    // }
    // else{
        frontLeft.move_velocity(forward + turn);
        middleLeft.move_velocity(forward + turn);	
        backLeft.move_velocity(forward + turn);
        frontRight.move_velocity(forward - turn);
        middleRight.move_velocity(forward - turn);
        backRight.move_velocity(forward - turn);
    // }
}

void Chassis::move(int leftSpeed, int rightSpeed){ // -600 - 600
    if(pto){
        frontLeft.move_velocity(leftSpeed);
        middleLeft.move_velocity(leftSpeed);	
        frontRight.move_velocity(rightSpeed);
        middleRight.move_velocity(rightSpeed);
    }
    else{
        frontLeft.move_velocity(leftSpeed);
        middleLeft.move_velocity(leftSpeed);	
        backLeft.move_velocity(leftSpeed);
        frontRight.move_velocity(rightSpeed);
        middleRight.move_velocity(rightSpeed);
        backRight.move_velocity(rightSpeed);
    }
}

void Chassis::moveDistance(int leftSpeed, int rightSpeed, int distance){
    resetMotorEncoders();
    while(abs(getDriveEncoder()) < abs(distance)){
        if(pto){
            frontLeft.move_velocity(leftSpeed);
            middleLeft.move_velocity(leftSpeed);	
            frontRight.move_velocity(rightSpeed);
            middleRight.move_velocity(rightSpeed);
        }
        else{
            frontLeft.move_velocity(leftSpeed);
            middleLeft.move_velocity(leftSpeed);	
            backLeft.move_velocity(leftSpeed);
            frontRight.move_velocity(rightSpeed);
            middleRight.move_velocity(rightSpeed);
            backRight.move_velocity(rightSpeed);
        }
    }
    stop();
}

void Chassis::stop(){
    if(pto){
        frontLeft.move_velocity(0);
        middleLeft.move_velocity(0);	
        frontRight.move_velocity(0);
        middleRight.move_velocity(0);
    }
    else{
        frontLeft.move_velocity(0);
        middleLeft.move_velocity(0);	
        backLeft.move_velocity(0);
        frontRight.move_velocity(0);
        middleRight.move_velocity(0);
        backRight.move_velocity(0);
    }
}

int Chassis::getVelocity(){
	return (frontLeft.get_actual_velocity() + middleLeft.get_actual_velocity() + frontRight.get_actual_velocity() + middleRight.get_actual_velocity())/4;
}

void Chassis::resetMotorEncoders(){
    pros::c::motor_tare_position(frontleftport);
    pros::c::motor_tare_position(frontrightport);
    pros::c::motor_tare_position(middleleftport);
    pros::c::motor_tare_position(middlerightport);
}

double Chassis::getDriveEncoder(){
    return (fabs(frontLeft.get_position()) +fabs(frontRight.get_position()))/2;
}

/**
* @brief Move chassis based on steering value
*        steerig 0, move forward
*        +/- 0-50, smooth turn (2 wheel turn)
*        +/- 50, pivot turn
*        +/- 50-100, one positive, one negative 2 wheel turn (rarely used)
*        +/- 100, on spot turn
*        +, turning left
*        -, turning right
*
* @param steering Steering direction of the robot
* @param speed Moving speed of the robot
*/
void Chassis::moveSteering(float steering, int speed)
{
	int leftSpeed;
	int rightSpeed;

	if (steering > 0) // turn left
	{
		leftSpeed = (steering * (-2) + 100) * speed / 100;
		rightSpeed = speed;
	}

	if(steering < 0) // turn right
	{
		leftSpeed = speed;
		rightSpeed = (steering * 2 + 100) * speed / 100;
	}

	if(steering == 0){ // go straight
		leftSpeed = speed;
		rightSpeed = speed;
	}
    move(leftSpeed, rightSpeed);
}

void Chassis::moveFowardMotor(double target, int speed, int decelZone){
    int counter = 0;
    resetMotorEncoders();
    forwardController.setTarget(target);
    keepStraight.setTarget(0);
    inertial.set_rotation(0);

    while(counter < 3){
        if(getDriveEncoder() < target - decelZone && target > 0){
            moveSteering(-keepStraight.compute(inertial.get_rotation()), speed);
        }
        else if(getDriveEncoder() < fabs(target) - fabs(decelZone) && target < 0){
            moveSteering(keepStraight.compute(inertial.get_rotation()), speed);
        }
        else{
            move(forwardController.compute(getDriveEncoder()), forwardController.compute(getDriveEncoder()));
        }
        if(getDriveEncoder() > target - 3 && getDriveEncoder()< target + 3){
            counter++;
        }
        else{
            counter = 0;
        }
        pros::delay(20);
    }
    stop();
} 

void Chassis::moveForward(double target, int speed){
    int counter = 0;
    resetMotorEncoders();
    forwardController.setTarget(target);
    keepStraight.setTarget(0);
    inertial.set_rotation(0);

    while(getDriveEncoder() < target){
        if(getDriveEncoder() < target && target > 0){
            moveSteering(-keepStraight.compute(inertial.get_rotation()), speed);
        }
        else if(getDriveEncoder() < fabs(target) && target < 0){
            moveSteering(keepStraight.compute(inertial.get_rotation()), speed);
        } 
        pros::delay(20);
    }
    stop();
} 

void Chassis::turnInertial(double target){
    inertial.set_rotation(0);
    turnController.setTarget(target);
    resetMotorEncoders();

    int counter = 0;
    const int speed_multiplier = 3;

    while(counter < 3){
        move(turnController.compute(inertial.get_rotation()) * speed_multiplier
        , -turnController.compute(inertial.get_rotation()) * speed_multiplier);
        
        if(inertial.get_rotation() > target - 1 && inertial.get_rotation() < target + 1){
            counter++;
        }
        else{
            counter = 0;
        }
        pros::delay(20);
    }
    stop();
}

void Chassis::turnInertialAbsolute(double target){
    turnController.setTarget(target);
    resetMotorEncoders();

    int counter = 0;
    const int speed_multiplier = 3;

    while(counter < 3){
        move(turnController.compute(inertial.get_rotation()) * speed_multiplier
        , -turnController.compute(inertial.get_rotation()) * speed_multiplier);
        
        if(inertial.get_rotation() > target - 1 && inertial.get_rotation() < target + 1){
            counter++;
        }
        else{
            counter = 0;
        }
        pros::delay(20);
    }
    stop();
}

void Chassis::turnInertialFast(double target){
    inertial.set_rotation(0);
    turnController.setTarget(target);
    resetMotorEncoders();

    int counter = 0;
    const int speed_multiplier = 3;

    while(counter < 3){
        move(turnController.compute(inertial.get_rotation()) * speed_multiplier
        , -turnController.compute(inertial.get_rotation()) * speed_multiplier);
        
        if(inertial.get_rotation() > target - 3 && inertial.get_rotation() < target + 3){
            counter++;
        }
        else{
            counter = 0;
        }
        pros::delay(20);
    }
    stop();
}

void Chassis::reset(){
    inertial.set_rotation(0);
}

/*
public void odometry() {
    oldRightPosition = currentRightPosition;
    oldLeftPosition = currentLeftPosition;
    //oldAuxPosition = currentAuxPosition;

    currentRightPosition = -encoderRight.getCurrentPosition();
    currentLeftPosition = encoderLeft.getCurrentPosition();
    //currentAuxPosition = encoderAux.getCurrentPosition();

    int dn1 = currentLeftPosition  - oldLeftPosition;
    int dn2 = currentRightPosition - oldRightPosition;
    //int dn3 = currentAuxPosition - oldAuxPosition;

    //double dtheta = cm_per_tick * ((dn2-dn1) / (LENGTH));
	double dtheta = cm_per_tick * (dn2-dn1);
    double dx = cm_per_tick * ((dn1+dn2) / 2.0);
    double dy = cm_per_tick * (dn3 + ((dn2-dn1) / 2.0));

    // telemetrydx = dx;
    // telemetrydy = dy;
    // telemetrydh = dtheta;

    pos.h += dtheta / 2;
    pos.x += dx * Math.cos(pos.h) - dy * Math.sin(pos.h);
    pos.y += dx * Math.sin(pos.h) + dy * Math.cos(pos.h);
}
*/

// double oldRightPosition = currentRightPosition;
// 		double oldLeftPosition = currentLeftPosition;
// 		//oldAuxPosition = currentAuxPosition;

// 		pros::delay(20);

// 		currentRightPosition = middleLeft.get_position();
// 		currentLeftPosition = middleRight.get_position();
// 		//currentAuxPosition = encoderAux.getCurrentPosition();

// 		int dn1 = currentLeftPosition  - oldLeftPosition;
// 		int dn2 = currentRightPosition - oldRightPosition;
// 		int dn3 = 0; //currentAuxPosition - oldAuxPosition;

// 		//double dtheta = cm_per_tick * ((dn2-dn1) / (LENGTH));
// 		double dtheta = constant * (dn2-dn1);
// 		double dx = constant * ((dn1+dn2) / 2.0);
// 		double dy = constant * (dn3 + ((dn2-dn1) / 2.0));

// 		// telemetrydx = dx;
// 		// telemetrydy = dy;
// 		// telemetrydh = dtheta;

// 		vect[counter][0] += dtheta / 2; 
// 		vect[counter][1] += dx * cos(vect[counter][0]) - dy * sin(vect[counter][0]);
// 		vect[counter][2] += dx * sin(vect[counter][0]) + dy * cos(vect[counter][0]);