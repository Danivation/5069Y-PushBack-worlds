#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)

// #define TRAFFIC_CONE_IN         DIGITAL_DOWN
// #define TRAFFIC_CONE_OUT        DIGITAL_RIGHT

#define CLAW_TOGGLE             master.get_digital(DIGITAL_B)
#define PISTON2_TOGGLE          master.get_digital(DIGITAL_Y)

#define INTAKE                  DIGITAL_R1
#define OUTTAKE                 DIGITAL_R2

#define LIFT_UP                 DIGITAL_L1
#define LIFT_DOWN               DIGITAL_L2

#define LOADER_TOGGLE           master.get_digital(DIGITAL_DOWN)
#define DESCORE_MID_TOGGLE      master.get_digital(DIGITAL_RIGHT)
#define DESCORE_WING_TOGGLE     master.get_digital(DIGITAL_Y)
#define INTAKE_RAISE_TOGGLE     master.get_digital(DIGITAL_LEFT)

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

void IntakeControl() {
    while (true) {
        if (master.get_digital(INTAKE)) {
            intake.move(127);
            cone.move(127);
        }
        if (master.get_digital(OUTTAKE)) {
            intake.move(-127);
            cone.move(-127);
        }
        if (!master.get_digital(INTAKE) && !master.get_digital(OUTTAKE)) {
            intake.brake();
            cone.brake();
        }
        delay(10);
    }
}

void LiftControl() {
    while (true) {
        if (master.get_digital(LIFT_UP)) {
            liftL.move(127);
            liftR.move(127);
        }
        if (master.get_digital(LIFT_DOWN)) {
            liftL.move(-127);
            liftR.move(-127);
        }
        if (!master.get_digital(LIFT_UP) && !master.get_digital(LIFT_DOWN)) {
            liftL.brake();
            liftR.brake();
        }
        delay(10);
    }
}

// void RollerControl() {
//     while (true) {
//         if (master.get_digital_new_press(ROLLER_UP)) {
//             roller.move(127);
//         }
//         if (master.get_digital_new_press(ROLLER_DOWN)) {
//             roller.move(-127);
//         }
//         if (!master.get_digital(ROLLER_UP) && !master.get_digital(ROLLER_DOWN)) {
//             roller.brake();
//         }
//         delay(10);
//     }
// }

void ClawControl() {
    while (true) {
        waitUntilCondition(!CLAW_TOGGLE);
        waitUntilCondition(CLAW_TOGGLE);
        claw.toggle();
        delay(10);
    }
}

void Piston2Control() {
    while (true) {
        waitUntilCondition(!PISTON2_TOGGLE);
        waitUntilCondition(PISTON2_TOGGLE);
        piston2.toggle();
        delay(10);
    }
}
