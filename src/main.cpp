#include "main.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <iostream>
#include <string.h>

using namespace pros;

Motor LF(6);
Motor RF(7);
Motor LB(8);
Motor RB(9);
Motor glock(10);//Motor 1 for shooter
Motor muzzel(11);//Motor 2 for shooter
Motor inserter(4);//Motor for receptical
Motor aimbotForSchoolShooting(15);//Motor for angle of shooter
Rotation sensingForBigBlackMenInYourArea(3);//Rotational Sensor for shooter
Controller master(E_CONTROLLER_MASTER);
int MAX_VOLTAGE_SPEED = 127;


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "Press against me harder daddy!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Wassup my ni-");
	delay(1000);
	pros::lcd::clear_line(1);

	pros::lcd::register_btn1_cb(on_center_button);
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
void competition_initialize() {}

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
void autonomous() {}


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
	bool isBlue = false;
	int x = 0;
	sensingForBigBlackMenInYourArea.set_position(0);

	LF.move(-100);
	LB.move(100);
	RF.move(100);
	RB.move(-100);

	delay(4100);

	LF.move(10);
	LB.move(-10);
	RF.move(-10);
	RB.move(10);
	delay(50);
	LF.move(0);
	LB.move(0);
	RF.move(0);
	RB.move(0);
}
