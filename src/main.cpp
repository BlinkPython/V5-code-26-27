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
pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Motor motor_port6(6);
pros::Motor motor_port7(7);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::register_btn1_cb(on_center_button);

	pros::MotorGroup left_drivetrain({1, 2, 3}, pros::MotorGearset::green);
	pros::MotorGroup right_drivetrain({1, 2, 3}, pros::MotorGearset::green);

	lemlib::Drivetrain drivetrain(\
	 &left_drivetrain,
	 &right_drivetrain,
	 10.5,
	 lemlib::Omniwheel::NEW_325,
	 360,
	 2);

	 pros::adi::Encoder VertEncoder('A', 'B', true);
	 pros::adi::Encoder HoriEncoder('C', 'D', true);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	std::string DisText = "imma sleep ok wait for it";

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
	std::string CompInitText = "LOCK IN BRO OR I WILL BREAK 10 SECONDS INTO THE ROUND";

	pros::lcd::initialize();
	pros::lcd::set_text(1, CompInitText);
}

// DO NOT WRITE AUTO HERE, instead use the dedicated auto.cpp file instead
/*
░█████╗░██╗░░░██╗████████╗░█████╗░███╗░░██╗
██╔══██╗██║░░░██║╚══██╔══╝██╔══██╗████╗░██║
███████║██║░░░██║░░░██║░░░██║░░██║██╔██╗██║
██╔══██║██║░░░██║░░░██║░░░██║░░██║██║╚████║
██║░░██║╚██████╔╝░░░██║░░░╚█████╔╝██║░╚███║
╚═╝░░╚═╝░╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚══╝
*/
void autonomous(){
	void AutonomousCode();
}

//  DO NOT WRITE CONTROLS HERE, instead use the dedicated opcontrol.cpp file for it

void opcontrol() {
    while (true) {
        // Read button states
        bool r1_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        bool r2_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

        // Check if BOTH R1 AND R2 are held
        if (r1_pressed && r2_pressed) {
            // Spin motors at full power (127)
            motor_port6.move(127);
            motor_port7.move(127);
        } else {
            // Stop motors if either button is released
            motor_port6.move(0);
            motor_port7.move(0);
        }

        // Output wattage to screen & terminal log
        double motor6_watts = motor_port6.get_power();
        double motor7_watts = motor_port7.get_power();

        pros::lcd::print(1, "Port 6 Power: %.2f W", motor6_watts);
        pros::lcd::print(2, "Port 7 Power: %.2f W", motor7_watts);

        pros::delay(20);
			pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1, -2, 3});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({-4, 5, -6});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage
		pros::delay(20);                               // Run for 20 ms then update
	}
    }
}