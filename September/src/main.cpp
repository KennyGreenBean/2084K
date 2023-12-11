#include "main.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_draw/lv_draw.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_btnm.h"
#include "display/lv_objx/lv_img.h"
#include "display/lv_objx/lv_label.h"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "api.h"
#include "okapi/api.hpp"

#include "SubSystems/Chassis.hpp"
#include "SubSystems/Intake.hpp"
#include "SubSystems/Flywheel.hpp"
#include "SubSystems/Catapult.hpp"
#include <cmath>
#include <functional>

#include "pros/apix.h"

using namespace okapi;

pros::Controller master(pros::E_CONTROLLER_MASTER); // Sets controller name to master

// Tells what ports each motor is on, what motor cartridge they have, and whether or not they are reversed
pros::Motor LEFT_FRONT(17, pros::E_MOTOR_GEARSET_06, true);//
pros::Motor LEFT_MIDDLE(10, pros::E_MOTOR_GEARSET_06, true);
pros::Motor LEFT_BACK(9, pros::E_MOTOR_GEARSET_06, true);

pros::Motor RIGHT_FRONT(19, pros::E_MOTOR_GEARSET_06, false);//
pros::Motor RIGHT_MIDDLE(3, pros::E_MOTOR_GEARSET_06, false);//
pros::Motor RIGHT_BACK(2, pros::E_MOTOR_GEARSET_06, false);

pros::Motor cataLeft(15, pros::E_MOTOR_GEARSET_18, false);//
pros::Motor cataRight(6, pros::E_MOTOR_GEARSET_18, true);//

pros::Motor intakeMotor(13, pros::E_MOTOR_GEARSET_18, true);

pros::IMU inertial(11);

pros::ADIDigitalOut PTO('F');
pros::ADIDigitalOut blooper ('H');
pros::ADIDigitalOut Flaps('G');
pros::ADIDigitalOut intak('E');
pros::ADIDigitalOut highHang('A');
pros::ADIDigitalOut lowHang('B');
pros::ADIDigitalOut blocker('C');

// class initialization
Chassis chassis = Chassis(LEFT_FRONT, RIGHT_FRONT, LEFT_MIDDLE, RIGHT_MIDDLE, LEFT_BACK, RIGHT_BACK, inertial);
Catapult catapult = Catapult(LEFT_BACK, RIGHT_BACK, cataLeft, cataRight);
Intake intake = Intake(intakeMotor);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */

void competition_initialize() {
}


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

bool currentToggle = false; // true = 6m drive, false = 4m drive

void ptoSwitch(){
	currentToggle = !currentToggle;
	PTO.set_value(currentToggle);
	chassis.ptoSwitch(currentToggle);
	catapult.ptoSwitch(currentToggle);
}

void ptoSetFalse(){
	currentToggle = false;
	PTO.set_value(currentToggle);
	chassis.ptoSwitch(false);
	catapult.ptoSwitch(false);
}

void ptoSetTrue(){
	currentToggle = true;
	PTO.set_value(currentToggle);
	chassis.ptoSwitch(true);
	catapult.ptoSwitch(true);
}

// void far6(){
// 	chassis.brake();
// 	catapult.brake();
// 	intake.brake();

// 	intake.spinIntake(true);
// 	chassis.moveDistance(100, 100, 70);	
// 	pros::delay(500);
// 	intake.stop();
// 	chassis.moveDistance(-600, -500, 1100);
// 	chassis.turnInertialFast(130);
// 	Flaps.set_value(true);
// 	chassis.move(130, 250);
// 	pros::delay(900);
// 	Flaps.set_value(false);
// 	chassis.turnInertial(30);
// 	chassis.reset();
// 	intake.spinIntake(false);
// 	chassis.move(300, 600);
// 	pros::delay(600);
// 	chassis.stop();

// 	chassis.moveDistance(-200, -200, 100);
// 	chassis.turnInertialAbsolute(-120);

// 	chassis.move(-400, -400);
// 	pros::delay(200);
// 	chassis.stop();
// 	///////
// 	chassis.moveFowardMotor(1000, 300, 0);
// 	chassis.turnInertial(45);
// 	intake.spinIntake(true);
// 	chassis.moveDistance(300, 300, 800);
// 	chassis.moveDistance(150, 150, 500);
// 	pros::delay(500);
// 	intake.stop();

// 	chassis.turnInertial(80);

// 	chassis.moveFowardMotor(500, 200, 0);
// 	chassis.turnInertial(90);
// 	Flaps.set_value(true);
// 	intake.spinIntake(false);
// 	chassis.move(400, 400);
// 	pros::delay(1000);
// 	chassis.stop();
// 	Flaps.set_value(false);
// }

void farSafe(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	intake.spinIntake(true);
	chassis.moveDistance(200, 200, 70);
	pros::delay(500);
	intake.stop();
	chassis.moveDistance(-400, -400, 1100);
	chassis.turnInertialFast(-217);
	chassis.moveDistance(200, 200, 400);

	intake.spinIntake(false);
	Flaps.set_value(true);
	chassis.moveDistance(100, 300, 400);
	Flaps.set_value(false);
	chassis.turnInertial(10);
	chassis.reset();
	intake.spinIntake(false);
	chassis.move(300, 600);
	pros::delay(1000);
	chassis.moveDistance(-300, -300, 300);
	intake.spinIntake(false);
	pros::delay(300);
	intake.stop();
	chassis.move(600, 600);
	pros::delay(500);
	chassis.stop();

	chassis.moveDistance(-200, -200, 200);
	chassis.turnInertialAbsolute(-105);
	intake.spinIntake(true);
	chassis.moveForward(2000, 400);
	chassis.moveDistance(200, 200, 400);
	pros::delay(400);
	intake.stop();
	
	chassis.turnInertialFast(80);
	chassis.moveForward(800, 400);
	chassis.turnInertialFast(100);
	intake.spinIntake(false);
	Flaps.set_value(true);
	chassis.move(600, 600);
	pros::delay(1000);
	chassis.stop();
}

void far5(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	Flaps.set_value(true);
	intake.spinIntake(true);
	pros::delay(200);
	Flaps.set_value(false);
	chassis.moveForward(2600, 500);
	chassis.moveForward(500, 200);
	pros::delay(500);
	intake.stop();
	chassis.turnInertialAbsolute(125);
	Flaps.set_value(true);
	intake.spinIntake(false);
	pros::delay(300);
	chassis.move(400, 400);
	pros::delay(300);
	intake.stop();
	pros::delay(600);
	chassis.stop();
	Flaps.set_value(false);
	chassis.moveDistance(-200, -200, 200);
	chassis.turnInertial(142);
	intake.spinIntake(true);
	chassis.moveDistance(400, 400, 750);
	chassis.moveDistance(200, 200, 330);
	pros::delay(700);
	intake.stop();

	chassis.turnInertial(-102);
	chassis.moveForward(2100, 400);
	// blooper.set_value(true);
	chassis.turnInertialFast(-90);
	// blooper.set_value(false);
	intake.spinIntake(false);
	Flaps.set_value(true);
	chassis.moveDistance(180, 200, 300);
	Flaps.set_value(false);
	chassis.moveDistance(-150, -200, 300);
	intake.spinIntake(false);
	chassis.move(300, 600);
	pros::delay(1300);
	chassis.moveDistance(-300, -300, 300);
	intake.spinIntake(false);
	pros::delay(300);
	intake.stop();
	chassis.move(600, 600);
}

void far3(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	chassis.moveFowardMotor(1950, 400, 200);

	chassis.turnInertialFast(90);

	intake.spinIntake(false);
	pros::delay(500);
	chassis.move(600, 600);
	pros::delay(400);
	chassis.moveDistance(-100, -100, 50);
	chassis.turnInertialFast(-135);
	intake.spinIntake(true);
	chassis.moveDistance(200, 200, 500);

	pros::delay(600);

	intake.stop();

	chassis.moveDistance(-200, -200, 300);	

	pros::delay(200);

	chassis.turnInertialFast(135);

	intake.spinIntake(false);
	pros::delay(500);

	intake.stop();
	chassis.move(400, 400);
	pros::delay(500);
	chassis.moveDistance(-200, -200, 200);

	pros::delay(100);

	chassis.turnInertial(150);

	intake.spinIntake(true);

	chassis.moveDistance(300, 300, 1250);

	pros::delay(600);

	intake.stop();

	chassis.moveDistance(-400, -400, 500);

	chassis.turnInertialFast(-150);

	intake.spinIntake(false);

	// Flaps.set_value(true);

	pros::delay(200);

	chassis.move(600, 600);

	pros::delay(400);

	// Flaps.set_value(false);
}

void far2(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	chassis.moveFowardMotor(1950, 400, 200);

	chassis.turnInertialFast(90);

	intake.spinIntake(false);
	pros::delay(500);
	chassis.move(600, 600);
	pros::delay(400);
	chassis.moveDistance(-100, -100, 50);
	chassis.turnInertialFast(-135);
	intake.spinIntake(true);
	chassis.moveDistance(200, 200, 500);

	pros::delay(600);

	intake.stop();

	chassis.moveDistance(-200, -200, 300);	

	pros::delay(200);

	chassis.turnInertialFast(135);

	intake.spinIntake(false);
	pros::delay(1000);
	intake.stop();
	chassis.move(400, 400);
	pros::delay(500);

	intake.stop();
	chassis.moveDistance(-200, -200, 200);

	pros::delay(100);

	chassis.turnInertial(122);

	chassis.move(400, 400);
	pros::delay(1300);

	chassis.stop();

	blooper.set_value(true);

	chassis.move(100, -100);
	pros::delay(1000);
	chassis.stop();
}

void far4(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	chassis.moveFowardMotor(1950, 400, 200);

	chassis.turnInertial(88);

	intake.spinIntake(false);
	pros::delay(200);
	chassis.moveDistance(-300, -300, 550);
	chassis.turnInertialFast(-140);
	intake.spinIntake(true);
	chassis.moveDistance(200, 200, 250);

	pros::delay(600);

	intake.stop();

	chassis.turnInertialFast(140);

	intake.spinIntake(false);
	Flaps.set_value(true);
	chassis.move(500, 500);
	pros::delay(800);

	Flaps.set_value(false);

	intake.stop();
	chassis.moveDistance(-200, -200, 200);

	pros::delay(100);

	chassis.turnInertial(144);

	intake.spinIntake(true);

	chassis.moveDistance(300, 300, 1450);

	pros::delay(600);

	intake.stop();

	chassis.moveDistance(-400, -300, 800);

	chassis.turnInertialFast(-150);

	intake.spinIntake(false);

	// Flaps.set_value(true);

	pros::delay(700);

	intake.stop();

	chassis.move(600, 600);
}

void closeShoot(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	blooper.set_value(true);
	catapult.spin();
	pros::delay(800);
	catapult.stop();

	chassis.turnInertial(-126);
	blooper.set_value(false);
	intake.spinIntake(false);
	chassis.moveDistance(200, 200, 1250);
	chassis.turnInertialFast(-20);
	chassis.moveDistance(200, 200, 600);
	blooper.set_value(true);
}

void close(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	intake.spinIntake(false);
	chassis.move(600, 300);
	pros::delay(1000);
	intake.stop();
	chassis.moveDistance(-200, -200, 300);
	chassis.turnInertial(-45);
	chassis.moveDistance(-300, -300, 500);
	chassis.turnInertial(-90);
	chassis.move(200, 200);
	pros::delay(300);
	chassis.stop();

	blooper.set_value(true);

	pros::delay(200);

	chassis.turnInertial(-90);

	blooper.set_value(false);

	intake.spinIntake(false);

	chassis.moveDistance(300, 300, 200);

	chassis.turnInertial(-37);

	chassis.moveDistance(300, 300, 1650);

	blooper.set_value(true);
}

void elimclose(){
	chassis.brake();
	catapult.brake();
	intake.brake();

	Flaps.set_value(true);
	pros::delay(200);
	Flaps.set_value(false);

	intake.spinIntake(true);
	chassis.moveDistance(500, 500, 1700);
	chassis.moveDistance(200, 200, 350);
	pros::delay(700);
	intake.stop();
	chassis.moveDistance(-500, -500, 2000);
	chassis.turnInertial(70);
	intake.spinIntake(false);
	pros::delay(400);
	chassis.moveDistance(-200, -200, 50);
	chassis.turnInertial(45);
	chassis.moveDistance(-300, -300, 300);
	Flaps.set_value(true);
	chassis.turnInertial(-60);
	Flaps.set_value(false);
	chassis.turnInertial(50);
	chassis.move(-200, -600);
	pros::delay(600);
	chassis.move(-600, -600);
	pros::delay(500);
	chassis.stop();
	chassis.moveDistance(290, 600, 1700);
	chassis.moveDistance(300, 300, 1600);
	blooper.set_value(true);
}

void skills(){
	chassis.brake();
	catapult.brake();
	intake.brake();
	ptoSetFalse();

	intak.set_value(false);

	chassis.moveDistance(300, 300, 300);
	chassis.turnInertial(-70);
	Flaps.set_value(true);
	catapult.spin();
	pros::delay(38000);
	catapult.stop();

	//-------------------------------------------------------------------------------

	Flaps.set_value(false);
	chassis.turnInertial(70);
	chassis.moveForward(650, -300);
	chassis.turnInertial(-50);

	chassis.moveDistance(-300, 	-300, 3300);

	chassis.moveDistance(-300, -80, 1500);
	chassis.moveDistance(-150, -270, 1500);
	chassis.moveDistance(-200, -200, 900);
	chassis.turnInertialFast(-90);
	chassis.move(-200, -200);
	pros::delay(500);

	
	Flaps.set_value(true);

	chassis.move(500, 500);

	pros::delay(1000);

	chassis.stop();

	Flaps.set_value(false);

	chassis.move(-500, -500);
	pros::delay(1000);
	chassis.stop();
	chassis.turnInertial(30);
	Flaps.set_value(true);
	chassis.move(400, 600);
	pros::delay(1000);
	chassis.stop();
	Flaps.set_value(false);
	chassis.move(-400, -400);
	pros::delay(2000);
	chassis.stop();
	chassis.turnInertial(-90);
	chassis.moveForward(2000, 400);
	chassis.turnInertial(120);
	Flaps.set_value(true);
	chassis.move(500, 500);
	pros::delay(1000);
	chassis.moveDistance(-200, -200, 300);
	Flaps.set_value(false);
	chassis.stop();
}

void skills2(){
	chassis.brake();
	catapult.brake();
	intake.brake();
	ptoSetFalse();

	intak.set_value(false);

	chassis.moveDistance(300, 300, 300);
	chassis.turnInertial(-70);
	Flaps.set_value(true);
	catapult.spin();
	pros::delay(70000);
	catapult.stop();

	//-------------------------------------------------------------------------------

	Flaps.set_value(false);
	chassis.turnInertial(70);
	chassis.moveForward(700, -300);
	chassis.turnInertial(-53);

	chassis.moveDistance(-300, 	-300, 3300);

	chassis.reset();

	chassis.move(-600, -350);
	pros::delay(2000);
	chassis.stop();

	chassis.turnInertialAbsolute(0);
	chassis.moveDistance(200, 300, 500);
	Flaps.set_value(true);
	chassis.move(500, 200);
	pros::delay(2000);
	chassis.move(600, 600);
	pros::delay(600);
	chassis.stop();
	chassis.move(-200, -200);
	pros::delay(500);
	chassis.move(500, 500);
	pros::delay(300);
	chassis.stop();
	Flaps.set_value(false);
	chassis.move(-300, -300);
	pros::delay(1000);
	chassis.stop();
	chassis.turnInertial(-90);	
	chassis.reset();
	chassis.moveDistance(300, 300, 1700);
	chassis.turnInertialAbsolute(130);
	Flaps.set_value(true);
	chassis.move(500, 600);
	pros::delay(1000);
	chassis.stop();

	chassis.move(-300, -300);
	pros::delay(500);
	chassis.stop();
	pros::delay(200);
	chassis.move(500, 500);
	pros::delay(500);
	chassis.stop();
	chassis.moveDistance(-300, -300, 300);
	
}

void autonomous() {
	ptoSetFalse();
	intak.set_value(false);
	// farSafe();
	// far5();
	// far4();
	// far3();
	// far2();
	// closeShoot();
	// close();
	// elimclose();
	// skills();
	skills2();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */


void opcontrol() {
	ptoSetFalse();
	chassis.brake();
	catapult.brake();
	intak.set_value(false);
	blooper.set_value(false);

	inertial.reset();

	bool flapsToggle = false;
	bool intakeToggle = false;
	bool blooperToggle = false;
	bool highHangToggle = false;
	bool lowHangToggle = false;
	bool blockerToggle = false;
	bool shootingToggle = false;

	// Loop that repeats forever until the program gets shut down
	while (true) { 
		// // Controls the movement of the drivetrain with the values of both joysticks
		chassis.inlineTelOp(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), 
		master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

		catapult.telOP(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1));

		intake.telOP(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1), 
		master.get_digital(pros::E_CONTROLLER_DIGITAL_R2));

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
			ptoSwitch();
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
			flapsToggle = !flapsToggle;
			Flaps.set_value(flapsToggle);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
			blooperToggle = !blooperToggle;
			blooper.set_value(blooperToggle);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){
			intakeToggle = !intakeToggle;
			intak.set_value(intakeToggle);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			highHangToggle = !highHangToggle;
			highHang.set_value(highHangToggle);
		}
		
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			lowHangToggle = !lowHangToggle;
			lowHang.set_value(lowHangToggle);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
			blockerToggle = !blockerToggle;
			blocker.set_value(blockerToggle);
		}


		// // Initialize the variable to store the temperature of motors
		pros::lcd::set_text(0, "TEMPERATURES");
		std::string temp = std::to_string((LEFT_FRONT.get_temperature() + LEFT_MIDDLE.get_temperature()
		 + RIGHT_FRONT.get_temperature() + RIGHT_MIDDLE.get_temperature())/4);

		// // Display the variable on the screen
		pros::lcd::set_text(1, "Drivetrain: " + temp);

		// repeat for all motors
		temp = std::to_string((LEFT_BACK.get_temperature() + RIGHT_BACK.get_temperature())/2);
		pros::lcd::set_text(2, "11W Catapult: " + temp);
		temp = std::to_string((cataLeft.get_temperature() + cataRight.get_temperature())/2);
		pros::lcd::set_text(3, "5.5W Catapult: " + temp);
		temp = std::to_string(intakeMotor.get_temperature());
		pros::lcd::set_text(4, "Intake: " + temp);

		
		pros::delay(20);
	}
}
