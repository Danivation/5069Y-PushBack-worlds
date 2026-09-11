#include "auton.hpp" // IWYU pragma: keep
#include "driver.hpp"
#include "lemlib-helpers.hpp" // IWYU pragma: keep
#include "main.h"
using namespace pros;

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

pros::Color get_color(pros::Optical* sensor) {
    // only return a color if a block is detected
    if ((int)sensor->get_proximity() <= 60) {
        return pros::Color::black;
    }

    float hue = sensor->get_hue();
    // sort hue into colors
    if (hue < 30 || hue > 330) {        // 330–360, 0–30
        return pros::Color::red;
    } else if (hue < 90) {              // 30–90
        return pros::Color::yellow;
    } else if (hue < 150) {             // 90–150
        return pros::Color::green;
    } else if (hue < 270) {             // 150–270
        return pros::Color::blue;
    } else {                            // 270–330
        return pros::Color::white;
    }
}
// returns true if exited due to color
bool waitUntilColor(pros::Optical* sensor, pros::Color color, int stopTime) {
    waitUntilFunction([&]{
        if (millis() >= stopTime) {
            return true;
        } else if (get_color(sensor) == color) {
            // wait 25 ms before checking again
            delay(25);
            if (get_color(sensor) == color) {
                return true;
            }
        }
        return false;
    });
    if (millis() >= stopTime) return false;
    else return true;
}


// wrist and lift in pin loading position
void intake_pin_pos() {
    // PIN LOADING POSITION
    setWristTo(-12);
    setLiftTo(15.6);
}

// wrist to scoring angle (where stack is vertical)
void score_pos() {
    setWristTo(125.2);
}

// wrist and lift to "matchload" position (for standing stacks)
void stack_pos() {
    setWristTo(117);
    setLiftTo(18);
}

// wrist and lift clasp in and lower (to grab standing stacks)
void clasp_pos() {
    setWristTo(100);
    setLiftTo(0);
}


void auton_none() {
    c_danielib.setPose(5.75, -2_tiles-13.25, 180);
    c_lemlib.setPose(5.75, -2_tiles-13.25, 180);

    // WRIST OUT OF WAY
    score_pos();
    cone.move(127);
    setLiftTo(0);
    delay(400);

    // DOUBLE TOGGLE
    c_danielib.driveForDistance(5, 500);
    lift_has_pid_control = false;
    delay(10);
    lift.move(127);
    delay(300);
    c_danielib.async().driveForDistance(-0.5, 300);
    delay(300);
    c_danielib.stopMovement();
    setLiftTo(40);

    // MOVE TO FIRST ALLIANCE GOAL
    c_lemlib.turnToHeading(230, 400);
    c_lemlib.moveToPoint(0.8_tiles, -2.1_tiles, 1000, {.forwards = false});
    setLiftTo(33);
    delay(350);
    score_pos();
    setLiftTo(25);

    // ON ALLIANCE GOAL, SCORE R/Y
    c_lemlib.waitUntilDone();
    c_lemlib.swingToHeading(270, DriveSide::LEFT, 500);
    setWristTo(131);
    setLiftTo(0);
    delay(440);
    setWristTo(95);
    delay(150);
    cone.move(-40);
    delay(300);

    // MOVE TO R/B CENTER DIAMOND STACK
    setWristTo(130);
    setLiftTo(20);
    delay(200);
    c_lemlib.moveToPoint(0_tiles, -2_tiles, 1000);
    c_lemlib.turnToHeading(180, 450);
    stack_pos();
    c_lemlib.moveToPoint(-0, -30, 1600, {.forwards = false});
    
    // GRAB DIAMOND R/B STACK
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-3, 1000, 15);
    cone.move(127);
    // delay(100);
    clasp_pos();
    delay(600);
    c_danielib.stopMovement();

    // LIFT UP AND MOVE TO NEUTRAL
    setLiftTo(35);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(75, 300);
    c_lemlib.moveToPoint(-0.85_tiles, -1.88_tiles, 1600, {.forwards = false});

    // SCORE R/B ON NEUTRAL
    c_lemlib.waitUntilDone();
    setLiftTo(20);
    delay(500);

    // LIFT OFF NEUTRAL + MOVE TO Y/Y STACK
    setLiftTo(40);
    cone.move(-50);
    delay(300);

    c_lemlib.moveToPoint(0.25_tiles, -1_tiles, 1600);
    c_lemlib.turnToHeading(-90, 700);
    stack_pos();

    c_lemlib.moveToPoint(0.72_tiles, -1.05_tiles, 1000, {.forwards = false});

    // GRAB Y/Y STACK
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-3, 1000, 15);
    cone.move(127);
    // delay(100);
    clasp_pos();
    delay(600);
    c_danielib.stopMovement();

}