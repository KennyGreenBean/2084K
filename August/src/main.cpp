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
pros::Motor LEFT_FRONT(20, pros::E_MOTOR_GEARSET_06, true);//
pros::Motor LEFT_MIDDLE(10, pros::E_MOTOR_GEARSET_06, true);
pros::Motor LEFT_BACK(9, pros::E_MOTOR_GEARSET_06, true);

pros::Motor RIGHT_FRONT(19, pros::E_MOTOR_GEARSET_06, false);//
pros::Motor RIGHT_MIDDLE(3, pros::E_MOTOR_GEARSET_06, false);//
pros::Motor RIGHT_BACK(2, pros::E_MOTOR_GEARSET_06, false);

pros::Motor cataLeft(15, pros::E_MOTOR_GEARSET_18, false);//
pros::Motor cataRight(1, pros::E_MOTOR_GEARSET_18, true);//

pros::Motor intakeMotor(13, pros::E_MOTOR_GEARSET_18, false);

pros::IMU inertial(19);

pros::ADIDigitalOut PTO('F');
pros::ADIDigitalOut blooper ('H');
pros::ADIDigitalOut Flaps('G');
pros::ADIDigitalOut intak('E');
pros::ADIDigitalOut highHang('A');
pros::ADIDigitalOut lowHang('B');

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

static bool leftAuto = false;
static bool rightAuto = false;
static bool skillsAuto = false;
static bool doNothingg = false;

lv_res_t leftSide(lv_obj_t * btn);
lv_res_t rightSide(lv_obj_t * btn);
lv_res_t skills(lv_obj_t * btn);
lv_res_t doNothing(lv_obj_t * btn);

lv_obj_t * leftButton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t * rightButton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t * skillsButton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t * nothingButton = lv_btn_create(lv_scr_act(), NULL);
lv_obj_t * textBox = lv_label_create(lv_scr_act(), NULL);

static lv_style_t selected;

void initialize() {
	lv_obj_t * label;

    lv_btn_set_action(leftButton, LV_BTN_ACTION_PR, leftSide);
	lv_obj_align(leftButton, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    label = lv_label_create(leftButton, NULL);
    lv_label_set_text(label, "Left Auto");

    lv_btn_set_action(rightButton, LV_BTN_ACTION_PR, rightSide);
	lv_obj_align(rightButton, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    label = lv_label_create(rightButton, NULL);
    lv_label_set_text(label, "Right Auto");

    lv_btn_set_action(skillsButton, LV_BTN_ACTION_PR, skills);
	lv_obj_align(skillsButton, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    label = lv_label_create(skillsButton, NULL);
    lv_label_set_text(label, "Skills");

    lv_btn_set_action(nothingButton, LV_BTN_ACTION_PR, doNothing);
	lv_obj_align(nothingButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    label = lv_label_create(nothingButton, NULL);
    lv_label_set_text(label, "Do Nothing");

	lv_obj_align(textBox, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
	lv_label_set_text(textBox, "Currently Selected: Nothing");

	selected.body.main_color = LV_COLOR_ORANGE;
	selected.text.color = LV_COLOR_WHITE;
	
	
}

lv_res_t leftSide(lv_obj_t * btn){
	rightAuto = false;
	skillsAuto = false;
	doNothingg = false;
	leftAuto = true;


	lv_btn_set_toggle(leftButton, true);

    lv_btn_set_style(leftButton, LV_BTN_STYLE_PR, &selected);

	return LV_RES_OK;
}
lv_res_t rightSide(lv_obj_t * btn){
	skillsAuto = false;
	doNothingg = false;
	leftAuto = false;
	rightAuto = true;

	lv_btn_set_toggle(rightButton, true);

	lv_btn_set_style(rightButton, LV_BTN_STYLE_PR, &selected);

	return LV_RES_OK;
}	
lv_res_t skills(lv_obj_t * btn){
	rightAuto = false;
	doNothingg = false;
	leftAuto = false;
	skillsAuto = true;

	lv_btn_set_toggle(skillsButton, true);

	lv_btn_set_style(skillsButton, LV_BTN_STYLE_PR, &selected);

	return LV_RES_OK;
}
lv_res_t doNothing(lv_obj_t * btn){
	rightAuto = false;
	leftAuto = false;
	skillsAuto = false;
	doNothingg = true;

	lv_btn_set_toggle(nothingButton, true);
	lv_btn_set_style(nothingButton, LV_BTN_STYLE_PR, &selected);

	return LV_RES_OK;
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
}

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

bool currentToggle = false; // false = 6m drive, true = 4m drive

void ptoSwitch(){
	currentToggle = !currentToggle;
	PTO.set_value(currentToggle);
	chassis.ptoSwitch(currentToggle);
	catapult.ptoSwitch(currentToggle);
}

void autonomous() {
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
	chassis.brake();
	catapult.brake();

	bool flapsToggle = false;
	bool intakeToggle = false;
	bool blooperToggle = false;
	bool highHangToggle = false;
	bool lowHangToggle = false;

	LEFT_FRONT.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	LEFT_MIDDLE.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	LEFT_BACK.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	RIGHT_FRONT.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	RIGHT_MIDDLE.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	RIGHT_BACK.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

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



		// // Initialize the variable to store the temperature of motors
		// std::string temp = std::to_string((Inertial.get_rotation()));

		// // Display the variable on the screen
		// pros::lcd::set_text(0, "Inertial: " + temp);

		// // repeat for all motors
		// temp = std::to_string(LEFT_FRONT.get_temperature());
		// pros::lcd::set_text(1, "Front Left: " + temp);
		// pros::lcd::set_text(2, "Middle Left: " + temp);
		// temp = std::to_string(LEFT_BACK.get_temperature());
		// pros::lcd::set_text(3, "Back Left: " + temp);
		// temp = std::to_string(RIGHT_FRONT.get_temperature());
		// pros::lcd::set_text(4, "Front Right: " + temp);
		// temp = std::to_string(catapultMotor.get_temperature());
		// pros::lcd::set_text(5, "catapult " + temp);
		
		pros::delay(20);
	}
}
