#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <strings.h>

using namespace pros;

Motor LF(6);
Motor RF(7);
Motor LB(8);
Motor RB(9);
Motor glock(10);//Motor 1 for shooter
Motor muzzel(11);//Motor 2 for shooter
Motor inserter(4);//Motor for receptical
Motor aimbotForSchoolShooting(15);//Motor for angle of shooter
Imu imu(21);
Rotation sensingForBigBlackMenInYourArea(3);//Rotational Sensor for shooter
Controller master(E_CONTROLLER_MASTER);
int MAX_VOLTAGE_SPEED = 127;
ADIDigitalOut ejaculate(1); 
ADIDigitalOut aimYourEjaculation(2);

void powerDrive(double T, double P, double S);
void drive(double ticks);
void turn(double degrees);

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
		pros::lcd::set_text(2, "");
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
	pros::lcd::set_text(1, "You're Balding Fucktard");
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
void autonomous() {
    double turn90 = 90 - (90/8);
    double inch = 80;

    delay(1000);
    drive(inch * 12);
    delay(500);
    turn(turn90);
    drive(inch);
    delay(2000);
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
    bool isBlue = false;
    int x = 0;
    bool angleCondition = true;
    bool shooterPosition = true;
    sensingForBigBlackMenInYourArea.set_position(0);
    while(true){
        int P = master.get_analog(ANALOG_LEFT_Y);
        int S = master.get_analog(ANALOG_LEFT_X);
        int T = master.get_analog(ANALOG_RIGHT_X);

        LF = T - P + S;
        LB = T + P - S;
        RF = T + P + S;
        RB = T - P - S;
        delay(50);

        if(master.get_digital_new_press(DIGITAL_R1) == 1) {
            glock = - 127;
            muzzel = 127;
        } else if(master.get_digital_new_press(DIGITAL_R2) == 1) {
            glock.move(0);
            muzzel.move(0);
        }

       

        if(master.get_digital(E_CONTROLLER_DIGITAL_Y)){inserter.move(100);}
        else{inserter.move(0);}

        if(master.get_digital_new_press(DIGITAL_L1) == 1){
            shooterPosition = !shooterPosition;
            ejaculate.set_value(shooterPosition);
        }

        if(master.get_digital_new_press(DIGITAL_A) == 1) {
            angleCondition = !angleCondition; 
            aimYourEjaculation.set_value(angleCondition);
        }

        pros::lcd::clear_line(1);
        }
    }
    //functions for moving in different directions based on encoder ticks and degrees
void powerDrive(double T, double P, double S){
    LF = T - P + S;
    LB = T + P - S;
    RF = T + P + S;
    RB = T - P - S;
}

void drive(double ticks){
    RF.tare_position();
    while(RF.get_position() <= ticks){
        powerDrive(0, 100, 0);
    }
    powerDrive(0, -10, 0);
    delay(50);
    powerDrive(0, 0, 0);
}

void turn(double degrees) {
    imu.tare_rotation();
    while(imu.get_rotation() <= degrees) {
        powerDrive(60, 0, 0);
    }
    powerDrive(-10, 0, 0);
    delay(50);
    powerDrive(0, 0, 0);
}
