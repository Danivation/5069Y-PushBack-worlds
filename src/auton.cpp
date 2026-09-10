#include "auton.hpp" // IWYU pragma: keep
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
void delayMid() {
    pros::delay(60);
}
void delayLong() {
    pros::delay(20);
}


void intake_pin() {
    // PIN LOADING POSITION
            setWristTo(-132);
            setLiftTo(15.6);
}


void auton_none() {
    
    c_lemlib.setPose(5.75, -2_tiles-14, 180);
    c_danielib.setPose(5.75, -2_tiles-14, 180);

    intake_pin();

    c_danielib.driveForDistance(-6, 500);
    c_danielib.driveForDistance(3, 800);
    c_danielib.driveForDistance(-6, 500);

    // pin intake and drive away

    intake.move(127);
    c_lemlib.moveToPoint(0, -1.5_tiles, 2000);
    

}