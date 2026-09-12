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



bool inPinPosition = false;
void cup_task() {

    // CHECK FOR INTAKE POSITION FIRST
    pros::Task autoCupTask {[&] {
        while (true) {
            if (inPinPosition && pin_dist.get_distance() < 150) {
                // wait 300 ms to make sure the pins still there
                delay(300);
                if (pin_dist.get_distance() < 150) {
                    // wait 50 ms to double check
                    delay(50);
                    if (pin_dist.get_distance() < 150) {
                        inPinPosition = false;
                        setLiftTo(16.8);
                        setWristTo(32);
                    }
                }
            }
            delay(10);
        }
    }};
}


// wrist and lift in pin loading position
void intake_pin_pos() {
    // PIN LOADING POSITION
    setLiftTo(15.6);
    setWristTo(-12);
    inPinPosition = true;
}


// wrist to scoring angle (where stack is vertical)
void score_pos() {
    inPinPosition = false;
    setWristTo(122);
}

// wrist and lift to "matchload" position (for standing stacks)
void stack_pos() {
    inPinPosition = false;
    setLiftTo(21);
    setWristTo(126);
}

// wrist and lift clasp in and lower (to grab standing stacks)
void clasp_pos() {
    inPinPosition = false;
    setWristTo(98);
    delay(50);
    setLiftTo(0);
}

void auton_none() {
    c_danielib.setPose(32.7, -59.9, 340.2);
    c_lemlib.setPose(32.7, -59.9, 340.2);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // SCORE R/Y IN ALLIANCE GOAL
    setLiftTo(0);
    // c_lemlib.moveToPoint(26.5, -57, 1000, {.minSpeed = 5, .earlyExitRange = 1});
    // c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(5.7, 700, 100, 0, false);
    stack_pos();
    delay(150);
    c_danielib.driveForDistance(-8, 100, 100, 0, false);

    // MOVE TO Y/Y STACK
    c_lemlib.turnToHeading(-130, 500);
    c_lemlib.moveToPoint(43, -51.5, 1000, {.forwards = false, .maxSpeed = 90, .minSpeed = 15, .earlyExitRange = 3});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-6, 400, 15);
    c_danielib.driveForDistance(2, 300);
    
    // GRAB Y/Y STACK
    cone.move(127);
    clasp_pos();
    delay(500);

    // LIFT UP AND MOVE TO GOAL
    setLiftTo(30);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(90, 500);
    c_lemlib.moveToPoint(30, -48, 1000, {.forwards = false});

    // SCORE Y/Y ON GOAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF GOAL
    setLiftTo(33);
    cone.move(-50);
    delay(300);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: OTHER ALLIANCE GOAL                                  */
    /* ---------------------------------------------------------------------------------------------- */

    // MOVE TO Y/Y STACK
    // c_lemlib.turnToHeading(30, 400);
    c_lemlib.moveToPoint(1.7_tiles, -1.8_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(140, 400);
    c_lemlib.moveToPoint(29, -29, 1000, {.forwards = false, .maxSpeed = 90, .minSpeed = 15, .earlyExitRange = 3});
    stack_pos();
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-6, 400, 15);
    c_danielib.driveForDistance(2, 300);
    
    // GRAB Y/Y STACK
    cone.move(127);
    clasp_pos();
    delay(500);

    // LIFT UP AND MOVE TO GOAL
    setLiftTo(30);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(45, -24, 1000, {.forwards = false});

    // SCORE Y/Y ON GOAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF GOAL
    setLiftTo(33);
    cone.move(-50);
    delay(300);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: INTAKE STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // MOVE TO FLOWER
    c_lemlib.moveToPoint(3, -1.4_tiles, 2000, {.minSpeed = 40, .earlyExitRange = 8});
    c_lemlib.moveToPoint(-15, -26, 2000, {.maxSpeed = 70});
    intake_pin_pos();
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    cup_task();
    delay(200);
    
    // GRAB FREE CUP
    c_lemlib.moveToPoint(-12, -35, 1000, {.forwards = false, .minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(50, 400);
    intake.move(127);
    cone.move(127);
    c_lemlib.moveToPoint(-2, -25, 2000, {.maxSpeed = 50, .minSpeed = 5, .earlyExitRange = 8});
    // c_lemlib.waitUntilDone();
    // c_danielib.driveForDistance(12, 1000, 15);
    inPinPosition = false;
    setWristTo(-5);
    
    // LIFT UP AND MOVE TO GOAL
    c_lemlib.turnToHeading(80, 300);
    c_lemlib.moveToPoint(-21, -43, 1500, {.forwards = false, .maxSpeed = 80});
    delay(200);
    setLiftTo(36);
    score_pos();

    // SCORE R/Y ON GOAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF GOAL
    setLiftTo(40);
    cone.move(-50);
    delay(300);
}

void auton_POSSIBLE_4PIN() {
    c_danielib.setPose(32.7, -59.9, 340.2);
    c_lemlib.setPose(32.7, -59.9, 340.2);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // SCORE R/Y IN ALLIANCE GOAL
    setLiftTo(0);
    // c_lemlib.moveToPoint(26.5, -57, 1000, {.minSpeed = 5, .earlyExitRange = 1});
    // c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(5.7, 700, 100, 0, false);
    stack_pos();
    delay(150);
    c_danielib.driveForDistance(-8, 100, 100, 0, false);

    // MOVE TO Y/Y STACK
    c_lemlib.turnToHeading(-130, 500);
    c_lemlib.moveToPoint(43, -51.5, 1000, {.forwards = false, .maxSpeed = 90, .minSpeed = 15, .earlyExitRange = 2.5});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-6, 500, 15);
    c_danielib.driveForDistance(2, 500);
    
    // GRAB Y/Y STACK
    cone.move(127);
    clasp_pos();
    delay(500);

    // LIFT UP AND MOVE TO GOAL
    setLiftTo(30);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(90, 500);
    c_lemlib.moveToPoint(30, -48, 1000, {.forwards = false});

    // SCORE Y/Y ON GOAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF GOAL
    setLiftTo(33);
    cone.move(-50);
    delay(300);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: NEUTRAL GOAL                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // MOVE TO Y/Y STACK
    // c_lemlib.turnToHeading(30, 400);
    c_lemlib.moveToPoint(1.7_tiles, -1.8_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(140, 400);
    c_lemlib.moveToPoint(29, -29, 1000, {.forwards = false, .maxSpeed = 90, .minSpeed = 15, .earlyExitRange = 2.5});
    stack_pos();
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-6, 500, 15);
    c_danielib.driveForDistance(2, 500);
    
    // GRAB Y/Y STACK
    cone.move(127);
    clasp_pos();
    delay(500);

    // LIFT UP AND MOVE TO GOAL
    setLiftTo(33);
    score_pos();
    delay(50);
    c_lemlib.moveToPoint(-19, -45.5, 2200, {.forwards = false});

    // SCORE Y/Y ON GOAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF GOAL
    setLiftTo(40);
    cone.move(-50);
    delay(300);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 3: INTAKE RED PIN                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // MOVE TO FLOWER
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(-15, -26, 1300, {.maxSpeed = 70});
    intake_pin_pos();
    cup_task();
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    delay(200);

    // // GRAB FREE CUP
    // c_lemlib.turnToHeading(90, 300);
    // c_lemlib.moveToPoint(0, -1_tiles, 1000);
    // c_lemlib.waitUntilDone();
    // intake_cup_pos();
    // delay(300);

    // // LIFT UP AND MOVE TO GOAL
    // c_lemlib.moveToPoint(-21, -43, 1000, {.forwards = false});
    // delay(200);
    // setLiftTo(40);
    // score_pos();

    // // SCORE R/Y ON GOAL
    // c_lemlib.waitUntilDone();
    // setLiftTo(25);
    // delay(550);

    // // LIFT OFF GOAL
    // setLiftTo(40);
    // cone.move(-50);
    // delay(300);
}

void auton_bad_3pin() {
    c_danielib.setPose(5, -59.2, 0);
    c_lemlib.setPose(5, -59.2, 0);

    // WRIST OUT OF WAY
    intake_pin_pos();

    // DOUBLE TOGGLE
    c_lemlib.turnToHeading(28, 250);
    c_lemlib.waitUntilDone();
    left_mg.move(-127);
    right_mg.move(-127);
    delay(500);
    left_mg.brake();
    right_mg.brake();

    // MOVE TO FIRST ALLIANCE GOAL
    intake.move(127);
    cone.move(127);
    c_danielib.driveForDistance(14, 350);
    setLiftTo(10);
    score_pos();
    c_lemlib.turnToHeading(-140, 300);
    c_lemlib.moveToPoint(0.8_tiles, -2.05_tiles, 1000, {.forwards = false});
    score_pos();
    setLiftTo(17);

    // ON ALLIANCE GOAL, SCORE R/Y
    delay(550);
    setWristTo(131);
    setLiftTo(0);
    delay(250);
    c_lemlib.cancelMotion();
    c_lemlib.swingToHeading(270, DriveSide::LEFT, 500);
    setWristTo(95);
    cone.move(-60);
    delay(150);
    c_lemlib.cancelMotion();

    // MOVE TO R/B CENTER DIAMOND STACK
    setWristTo(130);
    setLiftTo(20);
    delay(150);
    c_lemlib.moveToPoint(0, -2_tiles, 1200, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(180, 400);
    intake.brake();
    stack_pos();
    c_lemlib.moveToPoint(-0.2, -31.6, 1200, {.forwards = false, .minSpeed = 5, .earlyExitRange = 1.5});
    
    // GRAB DIAMOND R/B STACK
    c_lemlib.waitUntilDone();
    cone.move(127);
    clasp_pos();
    delay(200);
    c_danielib.async().driveForDistance(-3, 1000, 15);
    delay(300);
    c_danielib.stopMovement();

    // LIFT UP AND MOVE TO NEUTRAL
    setLiftTo(33);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(75, 300);
    c_lemlib.moveToPoint(-0.85_tiles, -1.88_tiles, 1350, {.forwards = false});

    // SCORE R/B ON NEUTRAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(550);

    // LIFT OFF NEUTRAL
    setLiftTo(40);
    cone.move(-50);
    delay(300);

    // MOVE TO Y/Y STACK
    c_lemlib.moveToPoint(-0.1_tiles, -1.05_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    delay(400);
    stack_pos();
    c_lemlib.waitUntilDone();
    c_danielib.turnToHeading(-85, 1000);
    lemlibDistReset({&left_beam, &back_beam}, 10, 10);
    stack_pos();
    c_lemlib.moveToPoint(14.4, -1_tiles, 1300, {.forwards = false, .minSpeed = 5, .earlyExitRange = 1.5});

    // GRAB Y/Y STACK
    c_lemlib.waitUntilDone();
    cone.move(127);
    clasp_pos();
    delay(200);
    c_danielib.async().driveForDistance(-3, 1000, 15);
    delay(300);
    c_danielib.stopMovement();
    
    // LIFT UP AND MOVE TO ALLIANCE
    setLiftTo(28);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(-20, 390);
    c_lemlib.moveToPoint(0.95_tiles, -1.88_tiles, 950, {.forwards = false});

    // SCORE Y/Y ON ALLIANCE
    c_lemlib.waitUntilDone();
    c_lemlib.swingToHeading(0, DriveSide::LEFT, 1000);
    setLiftTo(15);
    delay(600);

    // LIFT OFF AND MOVE
    setLiftTo(40);
    cone.move(-50);
    delay(300);
    c_lemlib.cancelMotion();
    c_lemlib.moveToPoint(0.9_tiles, -1.2_tiles, 1000, {.minSpeed = 30, .earlyExitRange = 6});

    // MOVE TO Y/Y STACK
    c_lemlib.turnToHeading(-40, 400);
    stack_pos();
    c_lemlib.moveToPoint(40.5, -44.7, 1500, {.forwards = false, .minSpeed = 5, .earlyExitRange = 1.5});

    // GRAB Y/Y STACK
    c_lemlib.waitUntilDone();
    cone.move(127);
    clasp_pos();
    delay(200);
    c_danielib.async().driveForDistance(-3, 1000, 15);
    delay(300);
    c_danielib.stopMovement();

    // LIFT UP AND MOVE TO NEUTRAL
    setLiftTo(45);
    score_pos();
    delay(50);
    c_lemlib.turnToHeading(-170, 500);
    c_lemlib.moveToPoint(1.95_tiles, -1.2_tiles, 1100, {.forwards = false});

    // SCORE Y/Y ON NEUTRAL
    c_lemlib.waitUntilDone();
    setLiftTo(15);
    delay(800);

    // LIFT OFF NEUTRAL
    setLiftTo(40);
    cone.move(-50);
    delay(1000);
}