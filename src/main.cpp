#include "main.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include <list>

using namespace pros;

Motor LF(6);
Motor RF(7);
Motor LB(8);
Motor RB(9);
Motor glock(5);//Motor 1 for shooter
Motor muzzel(1);//Motor 2 for shooter
Motor inserter(4);//Motor for receptical
Imu imu(21);
Controller master(E_CONTROLLER_MASTER);
int MAX_VOLTAGE_SPEED = 127;
ADIDigitalOut indexer(1);
ADIDigitalOut angleChanger(2);

void powerDrive(double T, double P, double S);
void drive(double ticks);
void negDrive(double ticks);
void negTurn(double ticks);
void rollerMove(double ticks);
void negRollerMove(double ticks);
void pneumatics();

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "You're Balding lol");
	} else {
		pros::lcd::clear_line(2);
	}
}
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "");
	imu.reset();
	delay(1000);
	pros::lcd::clear_line(1);

	pros::lcd::register_btn1_cb(on_center_button);
}
void disabled() {}
void competition_initialize() {}

void autonomous(){
	 drive(80 * 16);
	 negTurn(-87);
	 drive(80 * 5);
	 negRollerMove(-100);
	 delay(2000);
	 negDrive(80 * 10);
	 negTurn(-87);
	 delay(50);
	 glock.move(-127); muzzel.move(127);
	 delay(3500);
	 pneumatics();
}
 void opcontrol() {
     bool isBlue = false;
     int x = 0;
     bool angleCondition = true;
     bool shooterPosition = true;

     while(true){
         int P = master.get_analog(ANALOG_LEFT_Y);
         int S = master.get_analog(ANALOG_LEFT_X);
         int T = master.get_analog(ANALOG_RIGHT_X);

         LF = T - P + S;
         LB = T + P - S;
         RF = T + P + S;
         RB = T - P - S;
         delay(50);

				 if(master.get_digital(DIGITAL_R1)) {glock = - 100; muzzel = 100;}
				 else{glock.move(0); muzzel.move(0);}

         if(master.get_digital(E_CONTROLLER_DIGITAL_Y)){inserter.move(100);}
				 else if(master.get_digital(E_CONTROLLER_DIGITAL_B)){inserter.move(-100	);}
         else{inserter.move(0);}

         if(master.get_digital(DIGITAL_L1)) {indexer.set_value(1);}
						else{indexer.set_value(0);}

         if(master.get_digital_new_press(DIGITAL_A) == 1) {angleCondition = !angleCondition; angleChanger.set_value(angleCondition);}

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

void negDrive(double ticks){
	RF.tare_position();
	while(RF.get_position() >= ticks){
		powerDrive(0, -100, 0);
	}
	powerDrive(0, 10, 0);
	delay(50);
	powerDrive(0, 0, 0);
}

void negTurn(double ticks) {
	imu.tare_rotation();
	while(imu.get_rotation() >= ticks) {
		powerDrive(-60, 0, 0);
		// std::cout << imu.get_rotation();
	}
	powerDrive(10, 0, 0);
	delay(50);
	powerDrive(0, 0, 0);
}
void strafe(double ticks){
	RF.tare_position();
	while(RF.get_position() <= ticks){
		powerDrive(100, 0, 0);
	}
	powerDrive(-10, 0, 0);
	delay(50);
	powerDrive(0, 0, 0);
}

void rollerMove(double ticks) {
	inserter.tare_position();
	while(inserter.get_position() <= ticks) {
		inserter = 127;
	}
	inserter = 0;
}

void negRollerMove(double ticks) {
	inserter.tare_position();
	while(inserter.get_position() >= ticks) {
		inserter = -127;
	}
	inserter = 0;
}

void pneumatics(){
	indexer.set_value(1);
	delay(1000);
	indexer.set_value(1);
}
