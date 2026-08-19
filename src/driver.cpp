#include "danielib/exit.hpp"
#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)

// #define TRAFFIC_CONE_IN         DIGITAL_DOWN
// #define TRAFFIC_CONE_OUT        DIGITAL_RIGHT

#define CLAW_TOGGLE             master.get_digital(DIGITAL_X)

#define INTAKE                  DIGITAL_R1
#define OUTTAKE                 DIGITAL_R2

#define WRIST_UP                DIGITAL_UP
#define WRIST_DOWN              DIGITAL_DOWN

#define LIFT_UP                 DIGITAL_L1
#define LIFT_DOWN               DIGITAL_L2
#define LIFT_LOAD_MACRO         DIGITAL_B
#define LIFT_FLIP_MACRO         DIGITAL_Y

std::atomic<bool> driving = true;
std::atomic<bool> intake_control = true;
std::atomic<bool> wrist_control = true;
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

// right (17) returns posiitive, left (7) returns negative when winding clockwise
float getLiftPosition() {
    float leftMotorPosition = lift.get_position(0);
    float rightMotorPosition = lift.get_position(1);

    float avgPosition = (leftMotorPosition + rightMotorPosition) / 2;

    return avgPosition;
}

void moveLiftToPosition(float target, int timeout) {
    const int startTime = pros::millis();
    danielib::ExitCondition liftExit(liftPID.exitRange, liftPID.exitTime);

    float power = 0;
    float currentPosition = getLiftPosition();
    float error = 0;

    liftPID.reset();
    liftExit.reset();

    std::uint32_t time = pros::millis();
    while (pros::millis() < startTime + timeout && !liftExit.isDone()) {
        currentPosition = getLiftPosition();
        error = target - currentPosition;
        power = liftPID.update(error);
        liftExit.update(error);

        // clamp power
        // power = std::clamp(power, -127.0f, 127.0f);

        // move motors
        lift.move(power);

        // delay
        pros::Task::delay_until(&time, 10);
    }

    lift.brake();
}

void IntakeControl() {
    while (true) {
        if (intake_control) {
            if (master.get_digital(INTAKE)) {
                intake.move(127);
                cone.move(127);
            }
            else if (master.get_digital(OUTTAKE)) {
                intake.move(-127);
                cone.move(-127);
            }
            else {
                intake.brake();
                cone.brake();
            }
        }
        delay(10);
    }
}

void WristControl() {
    while (true) {
        if (wrist_control) {
            if (master.get_digital(WRIST_UP)) {
                wrist.move(127);
            }
            else if (master.get_digital(WRIST_DOWN)) {
                wrist.move(-127);
            }
            else {
                wrist.brake();
            }
        }
        delay(10);
    }
}

void LiftControl() {
    while (true) {
        if (master.get_digital(LIFT_LOAD_MACRO)) {
            moveLiftToPosition(1150);
            claw.extend();
            waitUntilCondition(!master.get_digital(LIFT_LOAD_MACRO));
        }
        else if (master.get_digital(LIFT_FLIP_MACRO)) {
            intake_control = false;
            delay(20);
            intake.move(127);
            cone.move(127);
            delay(300);
            moveLiftToPosition(1450);
            moveLiftToPosition(1230, 2000);
            delay(200);
            // delay(1000);
            claw.retract();
            moveLiftToPosition(1500);
            intake.brake();
            cone.brake();
            intake_control = true;
            waitUntilCondition(!master.get_digital(LIFT_FLIP_MACRO));
        }
        else if (master.get_digital(LIFT_UP)) {
            lift.move(127);
        }
        else if (master.get_digital(LIFT_DOWN)) {
            lift.move(-127);
        }
        else {
            lift.brake();
        }
        delay(10);
    }
}

void ClawControl() {
    while (true) {
        waitUntilCondition(!CLAW_TOGGLE);
        waitUntilCondition(CLAW_TOGGLE);
        claw.toggle();
        delay(10);
    }
}
