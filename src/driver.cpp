#include "danielib/exit.hpp"
#include "main.h"

/* ---------------------------------------------------------------------------------------------- */
/*                                       BUTTON ASSIGNMENTS                                       */
/* ---------------------------------------------------------------------------------------------- */

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)

#define INTAKE                  DIGITAL_R1
#define OUTTAKE                 DIGITAL_R2

#define LIFT_UP                 DIGITAL_L1
#define LIFT_DOWN               DIGITAL_L2

#define MATCHLOAD_MACRO         DIGITAL_DOWN
#define INTAKE_MACRO            DIGITAL_B
#define SCORING_MACRO           DIGITAL_Y
#define OUTTAKE_MACRO           DIGITAL_RIGHT

#define WRIST_UP_MANUAL         DIGITAL_UP
#define WRIST_DOWN_MANUAL       DIGITAL_LEFT

pros::Task* liftPIDTask = nullptr;
pros::Task* wristPIDTask = nullptr;

float liftCurrentTarget = 0;
float wristCurrentTarget = 0;

std::atomic<bool> driving = true;
std::atomic<bool> intake_control = true;
std::atomic<bool> wrist_control = true;
std::atomic<bool> lift_has_pid_control = false;
std::atomic<bool> wrist_has_pid_control = false;

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

/* ---------------------------------------------------------------------------------------------- */
/*                                        LIFT CONTROL PIDS                                       */
/* ---------------------------------------------------------------------------------------------- */

float getLiftPosition() {
    return (float)(lift_rot.get_position())/100.0f;
}

float getWristPosition() {
    return (float)(wrist_rot.get_position())/300.0f;
}

void startLiftWristPIDS() {
    liftPIDTask = new pros::Task {[&] {
        const int startTime = pros::millis();
        danielib::ExitCondition liftExit(liftPID.exitRange, liftPID.exitTime);

        float power = 0;
        float currentPosition = getLiftPosition();
        float error = 0;

        liftPID.reset();
        liftExit.reset();

        std::uint32_t time = pros::millis();
        while (/* pros::millis() < startTime + timeout && !liftExit.isDone() */ true) {
            currentPosition = getLiftPosition();
            error = liftCurrentTarget - currentPosition;
            power = liftPID.update(error);
            liftExit.update(error);

            // move motors
            if (lift_has_pid_control) lift.move(power);

            // delay
            pros::Task::delay_until(&time, 10);
        }

        lift.brake();
    }};

    wristPIDTask = new pros::Task {[&] {
        const int startTime = pros::millis();
        danielib::ExitCondition wristExit(wristPID.exitRange, wristPID.exitTime);

        float power = 0;
        float currentPosition = getWristPosition();
        float error = 0;

        wristPID.reset();
        wristExit.reset();

        std::uint32_t time = pros::millis();
        while (/* pros::millis() < startTime + timeout && !wristExit.isDone() */ true) {
            currentPosition = getWristPosition();
            error = wristCurrentTarget - currentPosition;
            power = wristPID.update(error);
            wristExit.update(error);

            // move motors
            if (wrist_has_pid_control) wrist.move(power);

            // delay
            pros::Task::delay_until(&time, 10);
        }

        wrist.brake();
    }};
}

void setLiftTo(float target) {
    lift_has_pid_control = true;
    liftCurrentTarget = target;
}

void setWristTo(float target) {
    wrist_has_pid_control = true;
    wristCurrentTarget = target;
}




/* ---------------------------------------------------------------------------------------------- */
/*                                    INTAKE AND WRIST CONTROLS                                   */
/* ---------------------------------------------------------------------------------------------- */



void IntakeControl() {
    while (true) {
        if (intake_control) {
            if (master.get_digital(INTAKE)) {
                cone.move(127);
                if (getLiftPosition() < 28) intake.move(127);
                else intake.brake();

                
                waitUntilCondition(!master.get_digital(INTAKE));
                intake.brake();
                cone.brake();
            }
            else if (master.get_digital(OUTTAKE)) {
                cone.move(-127);
                if (getLiftPosition() < 28) intake.move(-127);
                else intake.brake();

                waitUntilCondition(!master.get_digital(OUTTAKE));
                intake.brake();
                cone.brake();
            }
            else if (master.get_digital(OUTTAKE_MACRO)) {
                intake.move(-127);

                waitUntilCondition(!master.get_digital(OUTTAKE_MACRO));
                intake.brake();
            }
        }
        delay(10);
    }
}


bool readyForSecondPress = false;
bool inMatchLoadHoldPosition = false;

void WristControl() {
    bool inPinPosition = false;

    // CHECK FOR INTAKE POSITION FIRST
    pros::Task cupIntakeTask {[&] {
        while (true) {
            if (inPinPosition) {
                if (pin_dist.get_distance() < 150) {
                    // wait 300 ms to make sure the pins still there
                    delay(200);
                    if (pin_dist.get_distance() < 150) {
                        // wait 50 ms to double check
                        delay(100);
                        if (pin_dist.get_distance() < 150) {
                            inPinPosition = false;
                            setWristTo(30);
                            setLiftTo(17.7);
                        }
                    }
                }
            }
            delay(10);
        }
    }};


    while (true) {

        // MAIN BUTTON LOGIC
        if (master.get_digital_new_press(MATCHLOAD_MACRO)) {

            // MATCH LOAD POSITION
            if (!readyForSecondPress) {
                inPinPosition = false;
                inMatchLoadHoldPosition = false;
                readyForSecondPress = true;
                cone.brake();
                setLiftTo(14.9);
                setWristTo(128.5);

            } else {
                // MATCH LOAD MACRO DOWN
                inPinPosition = false;
                cone.move(127);
                setLiftTo(-1.5);
                setWristTo(119);
                delay(500);
                setLiftTo(9.5);
                setWristTo(135);
                readyForSecondPress = false;
                inMatchLoadHoldPosition = true;
            }

        } else if (master.get_digital_new_press(INTAKE_MACRO)) {

            // PIN LOADING POSITION
            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            setLiftTo(17.1);
            setWristTo(-10.5);
            int startTime = pros::millis();
            waitUntilCondition((getWristPosition() > -140 && getWristPosition() < -125 && getLiftPosition() > 5 && getLiftPosition() < 25) || pros::millis() + 500 > startTime);
            inPinPosition = true;

        } else if (master.get_digital_new_press(SCORING_MACRO)) {

            // SCORING (VERTICAL)
            readyForSecondPress = false;
            inPinPosition = false;
            inMatchLoadHoldPosition = false;
            setWristTo(121);
            if (getLiftPosition() < 22) setLiftTo(31);

        } else if (master.get_digital(WRIST_UP_MANUAL)) {
            inPinPosition = false;
            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            wrist_has_pid_control = false;
            delay(10);
            wrist.move(50);
            waitUntilCondition(!master.get_digital(WRIST_UP_MANUAL));
            wrist.brake();
            // setWristTo(getWristPosition());
            // wrist_has_pid_control = true;
        } else if (master.get_digital(WRIST_DOWN_MANUAL)) {
            inPinPosition = false;
            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            wrist_has_pid_control = false;
            delay(10);
            wrist.move(-50);
            waitUntilCondition(!master.get_digital(WRIST_DOWN_MANUAL));
            wrist.brake();
            // setWristTo(getWristPosition());
            // wrist_has_pid_control = true;
        } else {
            // wrist.brake();
        }
        delay(10);
    }
}

void LiftControl() {
    while (true) {
        if (master.get_digital(LIFT_UP)) {
            lift_has_pid_control = false;
            if (inMatchLoadHoldPosition) setWristTo(121);
            delay(10);
            lift.move(100);
            waitUntilCondition(!master.get_digital(LIFT_UP));
            lift.brake();
            inMatchLoadHoldPosition = false;
            readyForSecondPress = false;
        }
        else if (master.get_digital(LIFT_DOWN)) {
            lift_has_pid_control = false;
            delay(10);
            lift.move(-60);
            waitUntilCondition(!master.get_digital(LIFT_DOWN));
            lift.brake();
            inMatchLoadHoldPosition = false;
            readyForSecondPress = false;
        }
        else {
            // lift.brake();
        }
        delay(10);
    }
}
