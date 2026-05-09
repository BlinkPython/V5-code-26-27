#include "main.h"


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
		pros::lcd::set_text(2, "I was pressed!");
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
	std::string InitText = "heloo";

	pros::lcd::initialize();
	pros::lcd::set_text(1, InitText);

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	std::string DisText = "imma sleep ok wait";

	pros::lcd::initialize();
	pros::lcd::set_text(1, DisText);
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
	std::string CompInitText = "LOCK IN BRO OR I WILL BREAK ON YOU 10 SECONDS INTO THE ROUND";

	pros::lcd::initialize();
	pros::lcd::set_text(1, CompInitText);
}

// DO NOT WRITE AUTO HERE, instead use the dedicated auto.cpp file instead

void autonomous(){
	void AutonomousCode();
}

//  DO NOT WRITE CONTROLS HERE, instead use the dedicated opcontrol.cpp file for it

void opcontrol() {
	void Controls();
}