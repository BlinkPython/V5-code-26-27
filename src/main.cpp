#include "main.h"

// ===========================
// DEVICES
// ===========================

pros::Controller master(pros::E_CONTROLLER_MASTER);

// Drive
pros::MotorGroup leftDrive({1, 2, 3});
pros::MotorGroup rightDrive({-4, -5, -6});

// Cascade
pros::Motor lift(7);

// Intake / mechanism
pros::Motor mechanism(8);

// Pneumatic claw
pros::adi::DigitalOut claw('A');

// IMU
pros::Imu imu(10);


// ===========================
// LEMLIB SETUP
// ===========================

// Tracking wheels (CHANGE THESE)
pros::Rotation verticalEncoder(11);
pros::Rotation horizontalEncoder(12);

lemlib::TrackingWheel verticalTrackingWheel(
    &verticalEncoder,
    lemlib::Omniwheel::NEW_275,
    0
);

lemlib::TrackingWheel horizontalTrackingWheel(
    &horizontalEncoder,
    lemlib::Omniwheel::NEW_275,
    0
);


lemlib::Drivetrain drivetrain(
    &leftDrive,
    &rightDrive,
    10.5,
    lemlib::Omniwheel::NEW_325,
    360,
    2
);


lemlib::OdomSensors sensors(
    &verticalTrackingWheel,
    nullptr,
    &horizontalTrackingWheel,
    nullptr,
    &imu
);


lemlib::ControllerSettings lateralController(
    10,
    0,
    3,
    3,
    1,
    100,
    3
);


lemlib::ControllerSettings angularController(
    2,
    0,
    10,
    3,
    1,
    100,
    3
);


lemlib::Chassis chassis(
    drivetrain,
    lateralController,
    angularController,
    sensors
);


// ===========================
// CUSTOM FUNCTIONS
// ===========================


// ---- CLAW ----

void openClaw() {
    claw.set_value(true);
}

void closeClaw() {
    claw.set_value(false);
}


// ---- CASCADE ----

void liftTo(int position) {
    lift.move_absolute(position, 100);
}

void liftManual(int power) {
    lift.move(power);
}

void liftStop() {
    lift.brake();
}

void waitLift() {
    while(lift.is_busy()) {
        pros::delay(10);
    }
}


// ---- MECHANISM ----

void mechanismIn() {
    mechanism.move(127);
}

void mechanismOut() {
    mechanism.move(-127);
}

void mechanismStop() {
    mechanism.brake();
}


// ---- DRIVE ----

void driveTo(double x, double y) {
    chassis.moveToPoint(x, y, 2000);
}

void drivePose(double x, double y, double heading) {
    chassis.moveToPose(x, y, heading, 3000);
}

void turnTo(double heading) {
    chassis.turnToHeading(heading, 1000);
}

void waitDrive() {
    while(chassis.isInMotion()) {
        pros::delay(10);
    }
}


// ===========================
// GAME FUNCTIONS
// ===========================

void pickupPin() {
    openClaw();
    pros::delay(150);

    closeClaw();
    pros::delay(200);
}


void scorePin() {

    // CHANGE HEIGHT VALUES LATER
    liftTo(500);

    waitLift();

    openClaw();

    pros::delay(200);

    liftTo(0);
}


void resetRobot() {
    liftTo(0);
    closeClaw();
    mechanismStop();
}


// ===========================
// COMPETITION FUNCTIONS
// ===========================

void initialize() {

    pros::lcd::initialize();

    imu.reset();

    while(imu.is_calibrating()) {
        pros::delay(20);
    }

    chassis.calibrate();

    pros::lcd::set_text(1,"Robot Ready");
}


void disabled() {

}


void competition_initialize() {

}


void autonomous() {

    chassis.setPose(0,0,0);


    pickupPin();

    driveTo(24,12);

    scorePin();

}


void opcontrol() {

    while(true) {


        // DRIVE

        int forward = master.get_analog(
            pros::E_CONTROLLER_ANALOG_LEFT_Y
        );

        int turn = master.get_analog(
            pros::E_CONTROLLER_ANALOG_RIGHT_X
        );


        leftDrive.move(forward + turn);
        rightDrive.move(forward - turn);



        // LIFT CONTROL

        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            liftManual(127);
        }

        else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            liftManual(-127);
        }

        else {
            lift.brake();
        }



        // CLAW

        if(master.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_L1)) {

            openClaw();

        }


        if(master.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_L2)) {

            closeClaw();

        }



        pros::delay(20);
    }
}