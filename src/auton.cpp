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


void intake_pin() {
    // PIN LOADING POSITION
    setWristTo(-132+120);
    setLiftTo(15.6);
}

void score_stack() {
    setWristTo(4+120);
}


void auton_none() {
    c_danielib.setPose(5.75, -2_tiles-13.25, 180);
    c_lemlib.setPose(5.75, -2_tiles-13.25, 180);

    // WRIST OUT OF WAY
    score_stack();
    cone.move(127);
    setLiftTo(0);
    delay(400);

    // DOUBLE TOGGLE
    c_danielib.driveForDistance(6, 500);

    lift_has_pid_control = false;
    delay(10);
    lift.move(127);
    delay(1000);
    setLiftTo(40);

    // GOAL
    c_lemlib.turnToHeading(230, 400);
    c_lemlib.moveToPoint(0.75_tiles, -2.1_tiles, 2000, {.forwards = false});
    delay(700);
    setWristTo(110);
    setLiftTo(0);
    c_lemlib.waitUntilDone();
    cone.move(-60);
    delay(500);

    // MID STACK
    setLiftTo(20);
    delay(150);
    c_lemlib.moveToPoint(0, -2.3_tiles, 1000);
    c_lemlib.moveToPoint(2, -30, 2500, {.forwards = false});
    c_lemlib.waitUntilDone();
}