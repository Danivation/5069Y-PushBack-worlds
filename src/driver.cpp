#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)
#define INTAKE_TO_STORAGE       DIGITAL_R1
#define INTAKE_TO_LOW_GOAL      DIGITAL_R2
#define INTAKE_TO_SLOW_LOW_GOAL DIGITAL_A
#define INTAKE_TO_SLOW_MID_GOAL DIGITAL_X
#define INTAKE_TO_MID_GOAL      DIGITAL_L2
#define INTAKE_TO_HIGH_GOAL     DIGITAL_L1
#define LOADER_TOGGLE           master.get_digital(DIGITAL_DOWN)
#define DESCORE_MID_TOGGLE      master.get_digital(DIGITAL_RIGHT)//sidh was here
#define DESCORE_WING_TOGGLE     master.get_digital(DIGITAL_Y)
#define INTAKE_RAISE_TOGGLE     master.get_digital(DIGITAL_B)

bool driving = true;

void DrivetrainControl() {
    float throttle;
    float turn;
    while (true) {
        throttle = DeadBand(THROTTLE_AXIS, 2);
        turn = DeadBand(TURN_AXIS, 2);
        left_mg.move(throttle + turn);
        right_mg.move(throttle - turn);
        delay(10);
    }
}

// int lastPressed = 0;
// void IntakeControl() {
//     while (true) {
//         if (master.get_digital_new_press(INTAKE_TO_STORAGE)) lastPressed = 1;
//         else if (master.get_digital_new_press(INTAKE_TO_HIGH_GOAL)) lastPressed = 2;
//         else if (master.get_digital_new_press(INTAKE_TO_MID_GOAL)) lastPressed = 3;
//         else if (master.get_digital_new_press(INTAKE_TO_LOW_GOAL)) lastPressed = 4;

//         if (master.get_digital(INTAKE_TO_MID_GOAL)) {
//             hood.retract();
//             if (skillsSlow) {
//                 top.move(-40);
//                 bottom.move(40);
//             } else {
//                 bottom.move(127);
//                 top.move(-127);
//             }
//         } else if (master.get_digital(INTAKE_TO_HIGH_GOAL)) {
//             hood.extend();
//             bottom.move(127);
//             top.move(127);
//         } else if (master.get_digital(INTAKE_TO_STORAGE)) {
//             hood.retract();
//             bottom.move(127);
//             top.move(127);
//         } else if (master.get_digital(INTAKE_TO_LOW_GOAL)) {
//             hood.retract();
//             if (skillsSlow) {
//                 bottom.move(-75);
//                 top.brake();
//             } else {
//                 bottom.move(-127);
//                 top.move(-127);
//             }
//         } else if (master.get_digital_new_press(INTAKE_TO_SLOW_LOW_GOAL) && skillsSlow) {   // slow low goal macro
//             // driving = false;
//             // chassis.setPose(0, 0, 0);
//             // chassis.driveForDistance(-3.7, 600);
//             // driving = true;

//             // first fast score
//             intake_raise.extend();
//             hood.retract();
//             bottom.move(-70);
//             top.move(-40);
//             delay(600);

//             // medium score
//             bottom.move(-55);
//             top.brake();
//             delay(850);

//             // slow score end
//             bottom.move(-35);
//             top.brake();
//             delay(700);
//         } else if (master.get_digital_new_press(INTAKE_TO_SLOW_MID_GOAL) && skillsSlow) {   // slow mid goal macro
//             hood.retract();

//             bottom.move(-127);
//             top.move(-30);
//             delay(130);
//             bottom.brake();
//             delay(100);

//             // first fast score
//             bottom.move(60);
//             top.move(-55);
//             delay(700);

//             // slow score
//             bottom.move(50);
//             top.move(-35);
//             delay(2000);

//             // slowest score
//             bottom.move(30);
//             top.move(-18);
//             delay(800);
//         } else {
//             //hood.retract();
//             bottom.brake();
//             top.brake();
//             lastPressed = 0;
//         }
//         delay(15);
//     }
// }

// void LoaderControl() {
//     while (true) {
//         waitUntilCondition(!LOADER_TOGGLE);
//         waitUntilCondition(LOADER_TOGGLE);
//         loader.toggle();
//         delay(15);
//     }
// }

// void MidDescoreControl() {
//     while (true) {
//         waitUntilCondition(!DESCORE_MID_TOGGLE);
//         waitUntilCondition(DESCORE_MID_TOGGLE);
//         mid_descore.toggle();
//         delay(15);
//     }
// }

// void DescoreWingControl() {
//     while (true) {
//         waitUntilCondition(!DESCORE_WING_TOGGLE);
//         waitUntilCondition(DESCORE_WING_TOGGLE);
//         wing.toggle();
//         delay(15);
//     }
// }

// void IntakeRaiseControl() {
//     while (true) {
//         waitUntilCondition(!INTAKE_RAISE_TOGGLE);
//         waitUntilCondition(INTAKE_RAISE_TOGGLE);
//         intake_raise.toggle();
//         delay(15);
//     }
// }