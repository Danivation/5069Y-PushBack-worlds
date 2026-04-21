#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)
#define INTAKE_TO_STORAGE       DIGITAL_R1
#define INTAKE_TO_LOW_GOAL      DIGITAL_R2
#define INTAKE_TO_SLOW_LOW_GOAL DIGITAL_A
#define INTAKE_TO_SLOW_MID_GOAL DIGITAL_X
#define INTAKE_TO_MID_GOAL      DIGITAL_L2
#define INTAKE_TO_HIGH_GOAL     DIGITAL_L1
#define INTAKE_UNJAM_MACRO      DIGITAL_LEFT
#define LOADER_TOGGLE           master.get_digital(DIGITAL_DOWN)
#define DESCORE_MID_TOGGLE      master.get_digital(DIGITAL_RIGHT)
#define DESCORE_WING_TOGGLE     master.get_digital(DIGITAL_Y)
#define INTAKE_RAISE_TOGGLE     master.get_digital(DIGITAL_B)

std::atomic<bool> driving = true;
void DrivetrainControl() {
    float throttle;
    float turn;
    while (true) {
        if (driving) {
            throttle = DeadBand(THROTTLE_AXIS, 2);
            turn = DeadBand(TURN_AXIS, 2);
            left_mg.move(throttle + turn);
            right_mg.move(throttle - turn);
            delay(10);
        }
    }
}

std::atomic<int> lastPressed = 0;
void IntakeControl() {
    while (true) {
        if (master.get_digital_new_press(INTAKE_TO_STORAGE)) lastPressed = 1;
        else if (master.get_digital_new_press(INTAKE_TO_HIGH_GOAL)) lastPressed = 2;
        else if (master.get_digital_new_press(INTAKE_TO_MID_GOAL)) lastPressed = 3;
        else if (master.get_digital_new_press(INTAKE_TO_LOW_GOAL)) lastPressed = 4;

        if (master.get_digital(INTAKE_TO_MID_GOAL)) {
            hood.retract();
            intake_raise.retract();
            if (skillsSlow) {
                bottom.move(35);
                top.move(-40);
            } else {
                bottom.move(127);
                top.move(-127);
            }
        } else if (master.get_digital(INTAKE_TO_HIGH_GOAL)) {
            hood.extend();
            intake_raise.retract();
            bottom.move(127);
            top.move(127);
        } else if (master.get_digital(INTAKE_TO_STORAGE)) {
            hood.retract();
            intake_raise.retract();
            bottom.move(127);
            top.move(127);
        } else if (master.get_digital(INTAKE_TO_LOW_GOAL)) {
            hood.retract();
            intake_raise.extend();
            if (skillsSlow) {
                bottom.move(-30);
                top.move(-20);
            } else {
                bottom.move(-127);
                top.move(-127);
            }
        } else if (master.get_digital_new_press(INTAKE_UNJAM_MACRO)) {
            bottom.move(-127);
            delay(150);
            top.move(127);
            bottom.brake();
            delay(200);
            top.brake();
            bottom.brake();
        } else if (master.get_digital_new_press(INTAKE_TO_SLOW_LOW_GOAL) && skillsSlow) {   // slow low goal macro

            pros::Task lowMacro = pros::Task {[&] {
                driving = false;
                left_mg.brake();
                right_mg.brake();
                bottom.brake();
                top.brake();
                c_danielib.setPose(0, 0, 0);
                hood.retract();
                if (!intake_raise.is_extended()) {
                    intake_raise.extend();
                    delay(100);
                }

                // START MACRO

                // outtake
                bottom.move_velocity(-66);
                top.move(-30);
                delay(300);

                bottom.move_velocity(-60);
                delay(300);

                bottom.move_velocity(-50);
                top.brake();
                delay(600);
                c_danielib.async().driveForDistance(-2.0, 500);
                delay(300);

                bottom.move_velocity(-43);
                delay(650);
                c_danielib.driveForDistance(5, 300, 18);

                // BRAKE AND START DRIVING

                bottom.brake();
                top.brake();
                driving = true;
            
            }};
            pros::delay(10);
            waitUntilCondition(driving || DeadBand(THROTTLE_AXIS, 2) > 1 || DeadBand(TURN_AXIS, 2) > 1);
            if (!driving) {
                lowMacro.remove();
                bottom.brake();
                top.brake();
                delay(10);
                driving = true;
            }

        } else if (master.get_digital_new_press(INTAKE_TO_SLOW_MID_GOAL) && skillsSlow) {   // slow mid goal macro
            pros::Task midMacro = pros::Task {[&] {
                driving = false;
                left_mg.brake();
                right_mg.brake();
                bottom.brake();
                top.brake();
                c_danielib.setPose(0, 0, 0);
                hood.retract();

                // START MACRO

                
                // score mid goal
                c_danielib.async().driveForDistance(-8, 500, 15);
                bottom.move(-100);
                top.move(-30);
                delay(130);

                top.move_velocity(-95);
                bottom.move_velocity(90);
                delay(450);

                top.move_velocity(-90);
                bottom.move_velocity(65);
                delay(850);

                top.move_velocity(-85);
                bottom.move_velocity(58);
                delay(1000);

                top.move_velocity(-85);
                bottom.move_velocity(95);
                delay(1100);

                c_danielib.driveForDistance(-5, 350, 15);
                top.brake();
                bottom.brake();

                // BRAKE AND BACK UP

                bottom.brake();
                top.brake();
                c_danielib.driveForDistance(-5, 350, 15);

                // START DRIVING

                driving = true;
            }};
            pros::delay(10);
            waitUntilCondition(driving || DeadBand(THROTTLE_AXIS, 2) > 1 || DeadBand(TURN_AXIS, 2) > 1);
            if (!driving) {
                midMacro.remove();
                bottom.brake();
                top.brake();
                delay(10);
                driving = true;
            }
        } else {
            bottom.brake();
            top.brake();
            lastPressed = 0;
        }
        delay(15);
    }
}

void LoaderControl() {
    while (true) {
        waitUntilCondition(!LOADER_TOGGLE);
        waitUntilCondition(LOADER_TOGGLE);
        loader.toggle();
        delay(15);
    }
}

void MidDescoreControl() {
    while (true) {
        waitUntilCondition(!DESCORE_MID_TOGGLE);
        waitUntilCondition(DESCORE_MID_TOGGLE);
        mid_descore.toggle();
        delay(15);
    }
}

void DescoreWingControl() {
    while (true) {
        waitUntilCondition(!DESCORE_WING_TOGGLE);
        waitUntilCondition(DESCORE_WING_TOGGLE);
        wing.toggle();
        delay(15);
    }
}

void IntakeRaiseControl() {
    while (true) {
        waitUntilCondition(!INTAKE_RAISE_TOGGLE);
        waitUntilCondition(INTAKE_RAISE_TOGGLE);
        intake_raise.toggle();
        delay(15);
    }
}