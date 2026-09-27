#include "auton.hpp" // IWYU pragma: keep
#include "driver.hpp"
#include "lemlib-helpers.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
using namespace pros;




/* ---------------------------------------------------------------------------------------------- */
/*                                             AUTONS                                             */
/* ---------------------------------------------------------------------------------------------- */


void auton_test() {
    c_danielib.setPose(-6.7, -61.5, 0);
    c_lemlib.setPose(-6.7, -61.5, 0);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle - drive out and lift
    setLiftTo(65);
    c_danielib.async().driveForDistance(10, 800, 120);
    delay(300);
    setLiftTo(0);
    delay(300);
    c_danielib.stopMovement();

    // double toggle - drive back
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intakePin();
    c_danielib.waitUntilDone();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                    PART 1: ALLIANCE GOAL PIN                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // intake preload into cone
    intake.move(127);
    cone.move(127);

    // move to alliance
    c_lemlib.moveToPoint(-8, -2_tiles, 500);
    c_lemlib.turnToHeading(-110, 300);
    c_lemlib.moveToPoint(19, -47.5, 1200, {.forwards = false});
    delay(300);
    flipOut();
    delay(300);
    a0();
    c_lemlib.waitUntilDone();

    // score alliance goal
    score();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: YELLOW STACKS                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // grab inside stack
    c_lemlib.moveToPoint(5, -2_tiles, 450);
    c_lemlib.turnToHeading(-140, 200);
    load();
    c_lemlib.moveToPoint(24, -24, 2000, {.forwards = false, .maxSpeed = 80, .minSpeed = 30, .earlyExitRange = 12});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-8, 1000, 20);
    grab();




    delay(1000);



}




































































/* ---------------------------------------------------------------------------------------------- */
/*                                     MANUAL INTAKE POSITIONS                                    */
/* ---------------------------------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------------------------------- */
/*                              MATCH LOAD POSITION - LIFT AND WRIST                              */
/* ---------------------------------------------------------------------------------------------- */

void matchload_pos() {
    inPinPosition = false;
            setLiftTo(17.3);
            setWristTo(120);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                    SCORING POSITION - WRIST                                    */
/* ---------------------------------------------------------------------------------------------- */

// wrist to scoring angle (where stack is vertical)
void score_pos() {
    inPinPosition = false;
    setWristTo(120);
}

/* ---------------------------------------------------------------------------------------------- */
/*                            GRAB POSITION FOR STACKS - LIFT AND WRIST                           */
/* ---------------------------------------------------------------------------------------------- */

// wrist and lift to "matchload" position (for standing stacks)
void stack_pos() {
    inPinPosition = false;
    setLiftTo(22.5);
    setWristTo(120);
}

/* ---------------------------------------------------------------------------------------------- */
/*                              CLASP DOWN ON STACKS - LIFT AND WRIST                             */
/* ---------------------------------------------------------------------------------------------- */

// wrist and lift clasp in and lower (to grab standing stacks)
void clasp_pos() {
    inPinPosition = false;
    setWristTo(120);
    delay(50);
    setLiftTo(0);
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
    cupTask();
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
    cupTask();
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

void auton_flower_3pin_mir() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle - drive out and lift
    setLiftTo(65);
    c_danielib.async().driveForDistance(10, 800, 120);
    delay(300);
    setLiftTo(0);
    delay(300);
    c_danielib.stopMovement();

    // double toggle - drive back
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    c_danielib.waitUntilDone();



    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive fast up to cup + intake
    c_lemlib.moveToPoint(1, -28, 1500, {.minSpeed = 10, .earlyExitRange = 8.5});
    delay(150);
    intake.move(127);
    cone.move(127);
    cupTask();
    c_lemlib.waitUntilDone();

    // drive slow to intake cup
    c_lemlib.moveToPoint(0.2, -23.6, 500, {.maxSpeed = 50});
    inPinPosition = false;
    delay(10);

    // lift pin to unjam, intake cup, wait for cup
    setLiftTo(30);
    setWristTo(36);
    intake.brake();
    delay(100);
    intake_cup_pos();
    delay(50);
    intake.move(127);
    c_lemlib.waitUntilDone();

    // wiggle and back up to neutral goal
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.waitUntilDone();
    delay(200);
    c_lemlib.moveToPoint(20, -44, 1400, {.forwards = false, .maxSpeed = 100});
    delay(400);

    // grouping, flip out, wait to score
    setWristTo(20);
    delay(300);
    setLiftTo(38);
    score_pos();
    delay(400);

    // lower stack, score, lift off neutral
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(35);
    delay(150);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // move off neutral goal towards flower
    c_lemlib.moveToPoint(7, -1.2_tiles, 1500, {.minSpeed = 15, .earlyExitRange = 2});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(15);

    // turn to intake flower pin head on, wait for pin
    c_lemlib.turnToHeading(88, 450);
    c_lemlib.waitUntilDone();
    intake_pin_pos();
    c_lemlib.moveToPoint(13.5, -24, 1500);
    c_lemlib.waitUntilDone();
    delay(200);

    // back up to left alliance goal
    c_lemlib.moveToPoint(-20, -45, 1500, {.forwards = false});
    inPinPosition = false;
    delay(500);

    // flip out, wait to score
    intake.brake();
    setLiftTo(30);
    score_pos();
    delay(250);
    setLiftTo(14);
    delay(530);

    // score solo pin in alliance goal
    setLiftTo(0);
    delay(220);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);
    c_lemlib.cancelMotion();



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off alliance
    setWristTo(120);
    setLiftTo(20);
    delay(200);
}


void auton_4pin_3stack_elims() {
    c_danielib.setPose(-6.7, -61.5, 0);
    c_lemlib.setPose(-6.7, -61.5, 0);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle - drive out and lift
    setLiftTo(65);
    c_danielib.async().driveForDistance(10, 800, 120);
    delay(300);
    setLiftTo(0);
    delay(300);
    c_danielib.stopMovement();

    // double toggle - drive back
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    c_danielib.waitUntilDone();


    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // move to alliance goal
    c_lemlib.moveToPoint(-2, -2_tiles, 700, {.minSpeed = 40, .earlyExitRange = 8});
    delay(150);
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    // delay(300);
    cupTask();
    c_lemlib.turnToHeading(-50, 300);
    inPinPosition = false;
    delay(10);
    setLiftTo(25);
    delay(150);
    intake_cup_pos();

    // back up into alliance goal
    c_lemlib.moveToPoint(20, -48, 1000, {.forwards = false});
    inPinPosition = false;
    
    // flip out, wait to score
    intake.brake();
    setLiftTo(30);
    score_pos();
    delay(250);
    setLiftTo(14);
    delay(500);

    // score solo pin in alliance goal
    setLiftTo(0);
    delay(220);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);
    c_lemlib.cancelMotion();



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: INSIDE STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // setup inside stack movement
    c_lemlib.moveToPoint(6, -1.7_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(-120, 400);
    stack_pos();
    intake.brake();
    cone.move(127);

    // move to inside stack backwards
    c_lemlib.moveToPoint(22, -26.3, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7.5});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 700, 16);
    c_danielib.async().driveForDistance(2.7, 300);

    // pick up stack
    delay(150);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(38);
    score_pos();

    // move to goal
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(1_tiles, -45, 900, {.forwards = false});
    delay(900);

    // lower, score, lift off
    setLiftTo(5);
    delay(350);
    cone.move(-127);
    setLiftTo(45);
    delay(200);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(16, 500);
    // c_lemlib.moveToPoint(0, -2.1_tiles, 1000, {.minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.turnToHeading(-55, 300);
    stack_pos();
    intake.move(-127);
    cone.move(127);
    c_lemlib.moveToPoint(42.8, -43.5, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 6.5});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 700, 16);
    c_danielib.async().driveForDistance(2.7, 300);

    // pick up stack
    delay(150);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(47);
    score_pos();

    // back up to left alliance goal
    c_lemlib.turnToHeading(90, 400);
    c_lemlib.moveToPoint(28, -48, 1200, {.forwards = false});
    delay(700);
    
    // lower stack, score, lift off alliance
    setLiftTo(5);
    delay(350);
    cone.move(-127);
    setLiftTo(55);
    delay(200);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                    PART 4: MATCH LOAD SETUP                                    */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(2_tiles, -2_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(-45, 500);
    intake_pin_pos();
    c_lemlib.waitUntilDone();
}

void auton_4pin_3stack_elims_mir() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle - drive out and lift
    setLiftTo(65);
    c_danielib.async().driveForDistance(10, 800, 120);
    delay(300);
    setLiftTo(0);
    delay(300);
    c_danielib.stopMovement();

    // double toggle - drive back
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    c_danielib.waitUntilDone();


    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 1: ALLIANCE GOAL                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // move to alliance goal
    c_lemlib.moveToPoint(2, -2_tiles, 700, {.minSpeed = 40, .earlyExitRange = 8});
    delay(150);
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    // delay(300);
    cupTask();
    c_lemlib.turnToHeading(50, 300);
    inPinPosition = false;
    delay(10);
    setLiftTo(25);
    delay(150);
    intake_cup_pos();

    // back up into alliance goal
    c_lemlib.moveToPoint(-20, -48, 1000, {.forwards = false});
    inPinPosition = false;
    
    // flip out, wait to score
    intake.brake();
    setLiftTo(30);
    score_pos();
    delay(250);
    setLiftTo(14);
    delay(500);

    // score solo pin in alliance goal
    setLiftTo(0);
    delay(220);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);
    c_lemlib.cancelMotion();



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: INSIDE STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // lift off
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // setup inside stack movement
    c_lemlib.moveToPoint(-6, -1.7_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(120, 400);
    stack_pos();
    intake.brake();
    cone.move(127);

    // move to inside stack backwards
    c_lemlib.moveToPoint(-22, -26.3, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7.5});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 700, 16);
    c_danielib.async().driveForDistance(2.7, 300);

    // pick up stack
    delay(150);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(38);
    score_pos();

    // move to goal
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(-1_tiles, -45, 900, {.forwards = false});
    delay(900);

    // lower, score, lift off
    setLiftTo(5);
    delay(350);
    cone.move(-127);
    setLiftTo(45);
    delay(200);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(16, 500);
    // c_lemlib.moveToPoint(0, -2.1_tiles, 1000, {.minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.turnToHeading(55, 300);
    stack_pos();
    intake.move(-127);
    cone.move(127);
    c_lemlib.moveToPoint(-42.8, -43.5, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 6.5});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 700, 16);
    c_danielib.async().driveForDistance(2.7, 300);

    // pick up stack
    delay(150);
    cone.move(127);
    clasp_pos();
    delay(400);

    // lift up
    c_lemlib.cancelAllMotions();
    setLiftTo(47);
    score_pos();

    // back up to left alliance goal
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(-28, -48, 1200, {.forwards = false});
    delay(700);
    
    // lower stack, score, lift off alliance
    setLiftTo(5);
    delay(350);
    cone.move(-127);
    setLiftTo(55);
    delay(200);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                    PART 4: MATCH LOAD SETUP                                    */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(-2_tiles, -2_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(45, 500);
    intake_pin_pos();
    c_lemlib.waitUntilDone();
}



// FAR SIDE OUTSIDE STACK
void auton_one_stack_elims() {
    c_danielib.setPose(6.7, -61.5, 0);
    c_lemlib.setPose(6.7, -61.5, 0);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 0: DOUBLE TOGGLE                                     */
    /* ---------------------------------------------------------------------------------------------- */
    
    // double toggle - drive out and lift
    setLiftTo(65);
    c_danielib.async().driveForDistance(10, 800, 120);
    delay(300);
    setLiftTo(0);
    delay(300);
    c_danielib.stopMovement();

    // double toggle - drive back
    c_danielib.async().driveForDistance(-24, 800, 100);
    delay(100);
    intake_pin_pos();
    c_danielib.waitUntilDone();



    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive fast up to cup + intake
    c_lemlib.moveToPoint(1, -28, 1500, {.minSpeed = 10, .earlyExitRange = 9.5});
    delay(150);
    intake.move(127);
    cone.move(127);
    cupTask();
    c_lemlib.waitUntilDone();

    // drive slow to intake cup
    c_lemlib.moveToPoint(0.2, -23.6, 500, {.maxSpeed = 50});
    inPinPosition = false;
    delay(10);

    // lift pin to unjam, intake cup, wait for cup
    setLiftTo(30);
    setWristTo(36);
    intake.brake();
    delay(100);
    inPinPosition = false;
    intake_cup_pos();
    delay(50);
    intake.move(127);
    c_lemlib.waitUntilDone();

    // wiggle and back up to neutral goal
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.waitUntilDone();
    delay(1000);
    c_lemlib.moveToPoint(21, -44, 1400, {.forwards = false, .maxSpeed = 100});
    delay(300);

    // grouping, flip out, wait to score
    setWristTo(20);
    delay(200);
    setLiftTo(40);
    inPinPosition = false;
    score_pos();
    delay(1000);

    // lower stack, score, lift off neutral
    setLiftTo(5);
    delay(300);
    cone.move(-127);
    setLiftTo(35);
    delay(150);



    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // move off neutral goal towards flower
    c_lemlib.moveToPoint(7, -1.2_tiles, 1500, {.minSpeed = 15, .earlyExitRange = 2});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(15);

    // turn to intake flower pin head on, wait for pin
    c_lemlib.turnToHeading(88, 450);
    c_lemlib.waitUntilDone();
    intake_pin_pos();
    inPinPosition = false;
    c_lemlib.moveToPoint(13.5, -24, 1100);
    c_lemlib.waitUntilDone();
    intake.move(127);
    c_lemlib.turnToHeading(105, 100);
    c_lemlib.turnToHeading(75, 100);
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(3, 200);
    c_danielib.driveForDistance(-3, 200);
    c_danielib.driveForDistance(3, 200);
    c_lemlib.turnToHeading(105, 100);
    c_lemlib.turnToHeading(75, 200);
    c_lemlib.waitUntilDone();
    delay(800);

    // back up to left alliance goal
    c_lemlib.moveToPoint(-18.5, -46, 1500, {.forwards = false});
    inPinPosition = false;
    delay(500);

    // flip out, wait to score
    setLiftTo(30);
    score_pos();
    inPinPosition = false;
    delay(250);
    setLiftTo(14);
    delay(530);
    intake.brake();

    // score solo pin in alliance goal
    setLiftTo(0);
    delay(220);
    setWristTo(85);
    delay(150);
    cone.move(-100);
    delay(100);
    c_lemlib.cancelMotion();




    /* ---------------------------------------------------------------------------------------------- */
    /*                                     LAST: MATCH LOAD SETUP                                     */
    /* ---------------------------------------------------------------------------------------------- */
    // lift off alliance
    setWristTo(120);
    setLiftTo(20);
    delay(200);

    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_lemlib.turnToHeading(90, 150);
    c_lemlib.moveToPoint(-5, -1.9_tiles, 1000, {.minSpeed = 50, .earlyExitRange = 5});
    c_lemlib.turnToHeading(20, 300);
    matchload_pos();
    intake.move(-127);
    cone.move(127);
    c_lemlib.moveToPoint(-18, -2.55_tiles, 1000, {.forwards = false, .minSpeed = 50, .earlyExitRange = 9.5});
    c_lemlib.turnToHeading(90, 300);
    // delay(2000);
    // c_lemlib.moveToPoint(-50, -60, 1500, {.forwards = false});
    c_lemlib.waitUntilDone();


    
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
    cupTask();
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







