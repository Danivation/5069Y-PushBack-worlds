#include "danielib/exit.hpp"
#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)

// #define TRAFFIC_CONE_IN         DIGITAL_DOWN
// #define TRAFFIC_CONE_OUT        DIGITAL_RIGHT

#define CLAW_TOGGLE             master.get_digital(DIGITAL_B)

#define INTAKE                  DIGITAL_R1
#define OUTTAKE                 DIGITAL_R2

#define LIFT_UP                 DIGITAL_L1
#define LIFT_DOWN               DIGITAL_L2
#define LIFT_LOAD_MACRO         DIGITAL_Y

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
    while (pros::millis() < startTime + timeout) {
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
        delay(10);
    }
}

void LiftControl() {
    while (true) {
        if (master.get_digital(LIFT_LOAD_MACRO)) {
            moveLiftToPosition(990, 5000);
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
