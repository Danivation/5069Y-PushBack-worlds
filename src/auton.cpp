#include "auton.hpp" // IWYU pragma: keep
#include "driver.hpp"
#include "lemlib-helpers.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
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
bool isCupOrPinOnly = true;
void cup_task() {

    // CHECK FOR INTAKE POSITION FIRST
    pros::Task autoCupTask {[&] {
        while (true) {
            if (inPinPosition && pin_dist.get_distance() < 150) {
                // wait 300 ms to make sure the pins still there
                delay(300);
                if (pin_dist.get_distance() < 150) {
                    // wait 50 ms to double check
                    delay(100);
                    if (pin_dist.get_distance() < 150) {
                        inPinPosition = false;
                        setLiftTo(16.5);
                        if (isCupOrPinOnly) setWristTo(32);
                        else setWristTo(18);
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
    setWristTo(-4);
    inPinPosition = true;
}

// MANUAL cup position
void intake_cup_pos() {
    // CUP LOADING POSITION
    setWristTo(32);
    setLiftTo(16.5);
    inPinPosition = false;
}

// wrist to scoring angle (where stack is vertical)
void score_pos() {
    inPinPosition = false;
    setWristTo(125);
}

// wrist and lift to "matchload" position (for standing stacks)
void stack_pos() {
    inPinPosition = false;
    setLiftTo(22.5);
    setWristTo(120);
}

// wrist and lift clasp in and lower (to grab standing stacks)
void clasp_pos() {
    inPinPosition = false;
    setWristTo(98);
    delay(50);
    setLiftTo(0);
}

void matchload_pos() {
    
            inPinPosition = false;
            setLiftTo(17.5);
            setWristTo(120);
}

enum GoalType {alliance, neutral, center};

void fast_drop_prime(GoalType goalType) {
    cone.move(127);
    setWristTo(150);
    if (goalType == alliance) {
        setLiftTo(21.8);
    } else if (goalType == neutral) {
        setLiftTo(26.2);
    }
}

void fast_drop(int liftPos) {
    cone.move(-127);
    setWristTo(120);
    setLiftTo(liftPos);
}



/* ---------------------------------------------------------------------------------------------- */
/*                                             AUTONS                                             */
/* ---------------------------------------------------------------------------------------------- */



void auton_test() {
    c_danielib.setPose(0, -2_tiles, 90);
    c_lemlib.setPose(0, -2_tiles, 90);

    fast_drop_prime(neutral);
    delay(2000);
    c_lemlib.moveToPoint(-1_tiles, -2_tiles, 1500, {.forwards = false, .minSpeed = 15, .earlyExitRange = 9.5});
    c_lemlib.waitUntilDone();
    fast_drop(40);
    c_danielib.driveForDistance(24, 1500);
    c_danielib.waitUntilDone();
}


void auton_wesley_stacks_mir() {
    
    c_danielib.setPose(-6.7, -61.5, 0);
    c_lemlib.setPose(-6.7, -61.5, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle
    setLiftTo(75);
    c_danielib.async().driveForDistance(10, 800, 120, 0, false);
    delay(600);
    setLiftTo(0);
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    // delay(500);
    c_danielib.waitUntilDone();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(2, -2_tiles, 500, {.minSpeed = 40, .earlyExitRange = 8});
    delay(100);
    intake.move(127);
    cone.move(127);
    cup_task();
    c_lemlib.turnToHeading(60, 350);
    inPinPosition = false;
    delay(10);
    setLiftTo(23);
    delay(150);
    intake_cup_pos();

    // back up into alliance goal
    c_lemlib.moveToPoint(-20, -48, 1000, {.forwards = false});
    inPinPosition = false;
    delay(300);
    setLiftTo(15);
    score_pos();

    c_lemlib.waitUntilDone();

    // score alliance goal
    setLiftTo(0);
    delay(200);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       PART 2: OTHER STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // MOVE TO SHARED STACK!!
    c_lemlib.moveToPoint(-5, -1.7_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(130, 400);
    stack_pos();
    c_lemlib.moveToPoint(-22.5, -26.5, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 20, .earlyExitRange = 8});

    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 600, 15);
    c_danielib.async().driveForDistance(3, 400);

    // pick up stack
    delay(200);
    cone.move(127);
    clasp_pos();
    delay(300);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(37);
    score_pos();

    // score
    c_lemlib.turnToHeading(0, 300);
    c_lemlib.moveToPoint(-22, -46, 1000, {.forwards = false});

    // c_lemlib.waitUntilDone();
    delay(900);
    
    // score down on
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);

    /* ---------------------------------------------------------------------------------------------- */
    /*                             PART 3: SECOND YELLOW STACK TO NEUTRAL                             */
    /* ---------------------------------------------------------------------------------------------- */
    
    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // move to SECOND YELLOW STACK
    c_lemlib.moveToPoint(-9, -1.2_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(110, 300);
    stack_pos();
    c_lemlib.moveToPoint(-34, -36, 1500, {.forwards = false, .minSpeed = 35, .earlyExitRange = 6});
    c_lemlib.moveToPoint(-46, -48, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 20, .earlyExitRange = 8});

    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 600, 15);
    c_danielib.async().driveForDistance(3, 400);

    // pick up stack
    delay(300);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(40);
    score_pos();

    // GO SCORE ON NEUTRAL
    c_lemlib.turnToHeading(-140, 300);
    c_lemlib.moveToPoint(-24, -36, 1500, {.forwards = false, .minSpeed = 35, .earlyExitRange = 6});
    c_lemlib.moveToPoint(20, -46, 2000, {.forwards = false});


    // SCOREEE
    c_lemlib.waitUntilDone();
    // delay(900);
    
    // score down on
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);

}

// NON DRIVER SIDE AUTO
void auton_one_stack_flower_mir() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle
    setLiftTo(85);
    c_danielib.async().driveForDistance(10, 650, 120, 0, false);
    delay(350);
    setLiftTo(0);
    delay(300);
    intake_pin_pos();
    c_danielib.async().driveForDistance(-24, 1000, 100);
    // delay(500);
    c_danielib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(1, -28, 1500, {.minSpeed = 10, .earlyExitRange = 10});
    delay(180);
    intake.move(127);
    cone.move(127);
    cup_task();
    c_lemlib.waitUntilDone();
    c_lemlib.moveToPoint(0.2, -23.6, 1000, {.maxSpeed = 50});
    inPinPosition = false;
    delay(10);
    setLiftTo(20);
    delay(100);
    intake_cup_pos();
    delay(1150);

    //  back up to goal
    c_lemlib.moveToPoint(20, -44, 1400, {.forwards = false, .maxSpeed = 100});
    delay(175);

    // grouping
    setWristTo(20);
    delay(200);
    setLiftTo(38);
    score_pos();

    // c_lemlib.waitUntilDone();

    // SCIRE DOWN ON NEUTRAL
    delay(1100);
    setLiftTo(10);
    delay(300);
    cone.move(-127);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // lift up
    setLiftTo(35);
    delay(150);

    // move OFF NEUTRAL and TO FLOWER
    c_lemlib.moveToPoint(7, -1.2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 6});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(15);
    c_lemlib.turnToHeading(90, 330);
    c_lemlib.waitUntilDone();
    intake_pin_pos();
    c_lemlib.moveToPoint(14.1, -24.8, 1500);

    c_lemlib.waitUntilDone();

    delay(200);

    // back up back up!!
    c_lemlib.moveToPoint(-20, -45, 1500, {.forwards = false});
    inPinPosition = false;
    delay(500);
    setLiftTo(15);
    score_pos();
    intake.brake();

    c_lemlib.waitUntilDone();

    // score alliance goal
    setLiftTo(0);
    delay(200);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // MOVE TO SHARED STACK!!
    c_lemlib.moveToPoint(-5, -1.7_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(130, 400);
    stack_pos();
    c_lemlib.moveToPoint(-22.5, -26.5, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 15, .earlyExitRange = 10});

    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 600, 12);
    c_danielib.async().driveForDistance(3, 400);

    // pick up stack
    delay(300);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(37);
    score_pos();

    // score
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(-1_tiles, -45, 900, {.forwards = false});

    // c_lemlib.waitUntilDone();
    delay(800);
    
    // score down on
    setLiftTo(5);
    delay(350);
    cone.move(-127);
    setLiftTo(45);
    delay(200);

    
}



// NON DRIVER SIDE AUTO
void auton_one_stack_flower_farpin_mir() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle
    setLiftTo(75);
    c_danielib.async().driveForDistance(10, 650, 120, 0, false);
    delay(350);
    setLiftTo(0);
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    // delay(500);
    c_danielib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(1, -28, 1500, {.minSpeed = 10, .earlyExitRange = 10});
    delay(180);
    intake.move(127);
    cone.move(127);
    cup_task();
    c_lemlib.waitUntilDone();
    c_lemlib.moveToPoint(0.2, -23.6, 1000, {.maxSpeed = 50});
    inPinPosition = false;
    delay(10);
    setLiftTo(20);
    delay(100);
    intake_cup_pos();
    delay(1150);

    // back up to goal
    c_lemlib.moveToPoint(20, -44, 1400, {.forwards = false, .maxSpeed = 100});
    delay(175);

    // grouping
    setWristTo(20);
    delay(200);
    setLiftTo(38);
    score_pos();

    // c_lemlib.waitUntilDone();

    // SCIRE DOWN ON NEUTRAL
    delay(700);
    setLiftTo(10);
    delay(300);
    cone.move(-127);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // lift up
    setLiftTo(35);
    delay(150);

    // move OFF NEUTRAL and TO FLOWER
    c_lemlib.moveToPoint(7, -1.1_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 6});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(15);
    c_lemlib.turnToHeading(95, 330);
    c_lemlib.waitUntilDone();
    intake_pin_pos();
    c_lemlib.moveToPoint(14.1, -25.2, 1500);

    c_lemlib.waitUntilDone();

    delay(200);

    // back up back up!!
    c_lemlib.moveToPoint(-20, -45, 1500, {.forwards = false});
    inPinPosition = false;
    delay(500);
    setLiftTo(15);
    score_pos();
    intake.brake();

    delay(1080);

    // score alliance goal
    setLiftTo(0);
    delay(200);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    
    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // move to SECOND YELLOW STACK
    c_lemlib.turnToHeading(90, 150);
    c_lemlib.moveToPoint(-5, -1.9_tiles, 1000, {.minSpeed = 50, .earlyExitRange = 4});
    c_lemlib.turnToHeading(30, 300);
    stack_pos();
    c_lemlib.moveToPoint(-18, -2.55_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 10});
    c_lemlib.turnToHeading(90, 200);
    c_lemlib.moveToPoint(-26, -2.55_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 10});
    c_lemlib.turnToHeading(125, 200);
    c_lemlib.moveToPoint(-42.5, -51.5, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 9});

    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 500, 15);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(150);
    cone.move(127);
    clasp_pos();
    delay(300);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(37);
    score_pos();

    // score
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(-28, -48, 900, {.forwards = false});

    // c_lemlib.waitUntilDone();
    delay(700);
    
    // score down on
    setLiftTo(5);
    delay(250);
    cone.move(-127);
    setLiftTo(60);
    delay(200);

    
}

// FRONT AUTo
void auton_one_stack_flower() {
    c_danielib.setPose(-6.7, -61.5, 0);
    c_lemlib.setPose(-6.7, -61.5, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle
    setLiftTo(70);
    c_danielib.async().driveForDistance(10, 1000, 120, 0, false);
    delay(700);
    setLiftTo(0);
    delay(300);
    intake_pin_pos();
    c_danielib.async().driveForDistance(-24, 1000, 100);
    // delay(500);
    c_danielib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(-1, -28, 1500, {.minSpeed = 10, .earlyExitRange = 10});
    delay(180);
    intake.move(127);
    cone.move(127);
    cup_task();
    c_lemlib.waitUntilDone();
    c_lemlib.moveToPoint(-0.2, -23.6, 1000, {.maxSpeed = 50});
    inPinPosition = false;
    delay(10);
    setLiftTo(23);
    delay(150);
    intake_cup_pos();
    delay(1150);

    //  back up to goal
    c_lemlib.moveToPoint(-20, -44, 1400, {.forwards = false, .maxSpeed = 100});
    delay(250);
    setWristTo(20);
    delay(100);
    setLiftTo(38);
    score_pos();

    // c_lemlib.waitUntilDone();

    // SCIRE DOWN ON NEUTRAL
    delay(1100);
    setLiftTo(10);
    delay(300);
    cone.move(-127);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // lift up
    setLiftTo(35);
    delay(150);

    // move OFF NEUTRAL and TO FLOWER
    c_lemlib.moveToPoint(-7, -1.5_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 6});
    delay(250);
    intake.move(127);
    cone.move(127);
    intake_pin_pos();
    c_lemlib.turnToHeading(-60, 330);
    c_lemlib.moveToPoint(-13.5, -26, 1500);

    c_lemlib.waitUntilDone();

    delay(200);

    // back up back up!!
    c_lemlib.moveToPoint(20, -45, 1500, {.forwards = false});
    inPinPosition = false;
    delay(500);
    setLiftTo(15);
    score_pos();

    c_lemlib.waitUntilDone();

    // score alliance goal
    setLiftTo(0);
    delay(200);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // MOVE TO SHARED STACK!!
    c_lemlib.moveToPoint(5, -1.7_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(-130, 400);
    stack_pos();
    c_lemlib.moveToPoint(19, -27, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 20, .earlyExitRange = 8});

    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 600, 15);
    c_danielib.async().driveForDistance(3, 400);

    // pick up stack
    delay(300);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(37);
    score_pos();

    // score
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(1_tiles, -45, 900, {.forwards = false});

    // c_lemlib.waitUntilDone();
    delay(800);
    
    // score down on
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);
}



void auton_skills() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle
    setLiftTo(75);
    c_danielib.async().driveForDistance(10, 800, 120, 0, false);
    delay(600);
    setLiftTo(0);
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    // delay(500);
    c_danielib.waitUntilDone();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // move to alliance goal
    c_lemlib.moveToPoint(-2, -2_tiles, 700, {.minSpeed = 40, .earlyExitRange = 8});
    // delay(100);
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    delay(300);
    cup_task();
    c_lemlib.turnToHeading(-50, 300);
    inPinPosition = false;
    delay(10);
    setLiftTo(25);
    delay(150);
    intake_cup_pos();

    // back up into alliance goal
    c_lemlib.moveToPoint(20, -48, 1000, {.forwards = false});
    inPinPosition = false;
    delay(300);
    setLiftTo(18);
    score_pos();

    c_lemlib.waitUntilDone();

    // score alliance goal
    setLiftTo(0);
    delay(300);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                    PART 2: STACK ON ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */
    
    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);


    // wiggle around the goal to the stqack
    intake.brake();
    c_lemlib.turnToHeading(-90, 150);
    c_lemlib.moveToPoint(5, -1.9_tiles, 1000, {.minSpeed = 50, .earlyExitRange = 4});
    c_lemlib.turnToHeading(-140, 300);
    stack_pos();
    c_lemlib.moveToPoint(18, -1.51_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 7});
    c_lemlib.turnToHeading(-90, 200);
    c_lemlib.moveToPoint(36, -1.51_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 7});
    c_lemlib.turnToHeading(-55, 200);
    c_lemlib.moveToPoint(46, -44.5, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 9});

    // back up to align cup
    intake.brake();
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-10, 700, 15);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(250);
    cone.move(127);
    clasp_pos();
    delay(350);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(30);
    score_pos();

    // score
    c_lemlib.turnToHeading(90, 400);
    c_lemlib.moveToPoint(28, -48, 900, {.forwards = false});

    // c_lemlib.waitUntilDone();
    delay(700);
    
    // drop to score
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                 PART 3: MATCH LOAD ALLIANCE 3X                                 */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setLiftTo(35);
    delay(150);

    // macro move to the loader area
    c_lemlib.moveToPoint(2.45_tiles, -1.8_tiles, 1500);
    c_lemlib.turnToHeading(0, 400);
    matchload_pos();
    intake.brake();
    cone.brake();
    c_lemlib.waitUntilDone();
    lemlibDistReset({&right_beam});
    c_lemlib.moveToPoint(61, -60, 2500, {.forwards = false, .maxSpeed = 60});
    c_lemlib.waitUntilDone();
    cone.move(127);

    // into the loader down
    setLiftTo(0);
    delay(500);
    setLiftTo(10);

    // move back to the goal
    c_lemlib.moveToPoint(2.5_tiles, -1.9_tiles, 1500, {.minSpeed = 25, .earlyExitRange = 5});
    c_lemlib.turnToHeading(86, 300);
    setLiftTo(40);
    c_lemlib.moveToPoint(28, -48.5, 1000, {.forwards = false});

    c_lemlib.waitUntilDone();
    
    // drop to score
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);


    // PIN 2



    
    // lift off
    setLiftTo(35);
    delay(150);

    // macro move to the loader area
    c_lemlib.moveToPoint(2.45_tiles, -1.8_tiles, 1500);
    c_lemlib.turnToHeading(0, 400);
    matchload_pos();
    intake.brake();
    cone.brake();
    c_lemlib.waitUntilDone();
    lemlibDistReset({&right_beam});
    c_lemlib.moveToPoint(61, -60, 2500, {.forwards = false, .maxSpeed = 60});
    c_lemlib.waitUntilDone();
    cone.move(127);

    // into the loader down
    setLiftTo(0);
    delay(500);
    setLiftTo(10);

    // move back to the goal
    c_lemlib.moveToPoint(2.5_tiles, -1.9_tiles, 1500, {.minSpeed = 25, .earlyExitRange = 5});
    c_lemlib.turnToHeading(86, 300);
    setLiftTo(40);
    c_lemlib.moveToPoint(28, -48.5, 1000, {.forwards = false});

    c_lemlib.waitUntilDone();
    
    // drop to score
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(45);
    delay(200);

}












