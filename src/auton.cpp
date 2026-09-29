#include "auton.hpp" // IWYU pragma: keep
#include "driver.hpp"
#include "lemlib-helpers.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
using namespace pros;




/* ---------------------------------------------------------------------------------------------- */
/*                                             AUTONS                                             */
/* ---------------------------------------------------------------------------------------------- */

// void auton_2_2_stacks_elims_close() {
void auton_test1() {
    c_danielib.setPose(-6.7, -62.2, 0);
    c_lemlib.setPose(-6.7, -62.2, 0);


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
    // c_lemlib.moveToPoint(-7, -2.15_tiles, 550);
    c_danielib.driveForDistance(24, 300);
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(20, -2_tiles, 1300, {.forwards = false});
    delay(300);
    flipOut();
    delay(300);
    a0();
    delay(200);

    // score alliance goal
    intake.brake();
    cone.move(127);
    setLiftTo(0);
    delay(500);
    setWristTo(80);
    delay(200);
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 20);
    delay(400);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: INSIDE YELLOW                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // FIRST STACK
    intake.brake();
    c_lemlib.moveToPoint(3, -2_tiles, 500);
    // c_lemlib.turnToHeading(-140, 400);
    c_lemlib.turnToPoint(1_tiles, -1_tiles, 500, {.forwards = false});
    hover();
    cone.move(127);

    // stack point
    c_lemlib.moveToPoint(23, -22.6, 2000, {.forwards = false, .maxSpeed = 75, .minSpeed = 15, .earlyExitRange = 13});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-12, 1000, 15);
    c_danielib.async().driveForDistance(3, 300);
    delay(200);
    grabFlat();
    c_danielib.waitUntilDone();
    
    // score it
    a1();
    setWristTo(107);
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(24, -42, 1200, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 25);
    delay(200);
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 3: OUTSIDE YELLOW                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // SECOND STACK
    intake.brake();
    c_lemlib.moveToPoint(24, -26, 500);
    c_lemlib.turnToPoint(2_tiles, -2_tiles, 500, {.forwards = false});
    hover();
    cone.move(127);

    // stack point
    c_lemlib.moveToPoint(48, -48, 2000, {.forwards = false, .maxSpeed = 75, .minSpeed = 15, .earlyExitRange = 13});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-12, 750, 15);
    c_danielib.async().driveForDistance(3, 300);
    delay(200);
    grabFlat();
    c_danielib.waitUntilDone();

    // score it
    a2();
    c_lemlib.turnToHeading(90, 400);
    c_lemlib.moveToPoint(30, -48, 1200, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 25);
    delay(200);
    score();
    c_danielib.stopMovement();
}




void auton_test() {
    c_danielib.setPose(-6.7, -62.2, 0);
    c_lemlib.setPose(-6.7, -62.2, 0);


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

    // move to alliance goal
    c_danielib.driveForDistance(24, 300);
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(20, -2_tiles, 1300, {.forwards = false});
    delay(300);
    flipOut();
    delay(300);
    a0();

    // score alliance goal
    intake.brake();
    cone.move(127);
    setLiftTo(0);
    delay(500);
    setWristTo(80);
    delay(200);
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 20);
    delay(400);
    intake.brake();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: INSIDE STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */


    // setup inside stack movement
    c_lemlib.turnToHeading(-45, 250);
    c_lemlib.moveToPoint(3, -42, 1000, {.minSpeed = 50, .earlyExitRange = 5});
    c_lemlib.turnToHeading(-110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(21.6, -27.8, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 8});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 800, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(150);
    grabFlat();

    // lift up
    c_lemlib.waitUntilDone();
    a1();

    // move to goal
    c_lemlib.turnToHeading(-7, 400);
    c_lemlib.moveToPoint(24, -44.5, 1200, {.forwards = false, .minSpeed = 30, .earlyExitRange = 2.5});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 30);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(12, 350);
    c_lemlib.turnToHeading(-48, 300);
    hover();
    c_lemlib.moveToPoint(44.3, -45.3, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 8});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 800, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(150);
    grabFlat();

    // lift up
    c_lemlib.waitUntilDone();
    a2();

    // back up to left alliance goal
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(27, -48, 1200, {.forwards = false, .minSpeed = 30, .earlyExitRange = 2.5});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 30);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 1: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_lemlib.moveToPoint(2.45_tiles, -1.8_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 3.5});
    c_lemlib.turnToHeading(0, 500);
    load();
    c_lemlib.waitUntilDone();

    // back into loader slow
    c_lemlib.moveToPoint(58.8, -62, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 3});
    c_lemlib.moveToPoint(58.8, -70, 500, {.forwards = false, .maxSpeed = 25});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grabFlat();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 1: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    a3();
    c_lemlib.moveToPoint(2.5_tiles, -2.1_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(87, 400);
    c_lemlib.moveToPoint(27, -2_tiles, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 3});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(150);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 2: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_lemlib.moveToPoint(2.45_tiles, -1.8_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 3.5});
    c_lemlib.turnToHeading(0, 500);
    load();
    c_lemlib.waitUntilDone();

    // back into loader slow
    c_lemlib.moveToPoint(58.8, -62, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 3});
    c_lemlib.moveToPoint(58.8, -70, 500, {.forwards = false, .maxSpeed = 25});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grabFlat();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 2: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    a4();
    c_lemlib.moveToPoint(2.5_tiles, -2.1_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(87, 400);
    c_lemlib.moveToPoint(27, -2_tiles, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 3});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(150);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 3: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_lemlib.moveToPoint(2.45_tiles, -1.8_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 3.5});
    c_lemlib.turnToHeading(0, 500);
    load();
    c_lemlib.waitUntilDone();

    // back into loader slow
    c_lemlib.moveToPoint(58.8, -62, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 3});
    c_lemlib.moveToPoint(58.8, -70, 500, {.forwards = false, .maxSpeed = 25});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grabFlat();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 3: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    a4();
    c_lemlib.moveToPoint(2.5_tiles, -2.1_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    c_lemlib.turnToHeading(87, 400);
    c_lemlib.moveToPoint(27, -2_tiles, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 3});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(150);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();

}




void auton_skills() {
    c_danielib.setPose(-6.7, -62.2, 0);
    c_lemlib.setPose(-6.7, -62.2, 0);


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
    // c_lemlib.moveToPoint(-7, -2.15_tiles, 550);
    c_danielib.driveForDistance(24, 300);
    c_lemlib.turnToHeading(-90, 400);
    c_lemlib.moveToPoint(20, -2_tiles, 1300, {.forwards = false});
    delay(300);
    flipOut();
    delay(300);
    a0();
    delay(200);

    // score alliance goal
    intake.brake();
    cone.move(127);
    setLiftTo(0);
    delay(500);
    setWristTo(80);
    delay(200);
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 20);
    delay(400);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 2: INSIDE YELLOW                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // FIRST STACK
    intake.brake();
    c_lemlib.moveToPoint(3, -2_tiles, 500);
    // c_lemlib.turnToHeading(-140, 400);
    c_lemlib.turnToPoint(1_tiles, -1_tiles, 500, {.forwards = false});
    hover();
    cone.move(127);

    // stack point
    c_lemlib.moveToPoint(23, -22.6, 2000, {.forwards = false, .maxSpeed = 75, .minSpeed = 15, .earlyExitRange = 13});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-12, 1000, 15);
    c_danielib.async().driveForDistance(3, 300);
    delay(200);
    grabFlat();
    c_danielib.waitUntilDone();
    
    // score it
    a1();
    setWristTo(107);
    c_lemlib.turnToHeading(0, 400);
    c_lemlib.moveToPoint(24, -42, 1200, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 25);
    delay(200);
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 3: OUTSIDE YELLOW                                     */
    /* ---------------------------------------------------------------------------------------------- */

    
    // SECOND STACK
    intake.brake();
    c_lemlib.moveToPoint(24, -26, 500);
    c_lemlib.turnToPoint(2_tiles, -2_tiles, 500, {.forwards = false});
    hover();
    cone.move(127);

    // stack point
    c_lemlib.moveToPoint(48, -48, 2000, {.forwards = false, .maxSpeed = 75, .minSpeed = 15, .earlyExitRange = 13});
    c_lemlib.waitUntilDone();
    c_danielib.driveForDistance(-12, 750, 15);
    c_danielib.async().driveForDistance(3, 300);
    delay(200);
    grabFlat();
    c_danielib.waitUntilDone();

    // score it
    a2();
    c_lemlib.turnToHeading(90, 400);
    c_lemlib.moveToPoint(30, -48, 1200, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 25);
    delay(200);
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                    PART 3: MATCHLOAD REPEAT                                    */
    /* ---------------------------------------------------------------------------------------------- */


    // matchload sequence
    c_lemlib.turnToHeading(90, 200);
    c_lemlib.moveToPoint(2.52_tiles, -2_tiles, 2000);
    delay(200);
    load();
    c_lemlib.waitUntilDone();
    c_danielib.turnToHeading(0, 600);
    lemlibDistReset({&right_beam});
    c_lemlib.moveToPoint(2.49_tiles, -2.7_tiles, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 8});
    c_lemlib.moveToPoint(2.49_tiles, -2.8_tiles, 800, {.forwards = false, .maxSpeed = 30});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam});
    c_danielib.driveForDistance(1, 150);
    grab();


    // scoring sequence
    c_lemlib.moveToPoint(2.4_tiles, -2_tiles, 1500, {.minSpeed = 15, .earlyExitRange = 2});
    delay(100);
    a3();
    c_lemlib.turnToHeading(90, 480);
    c_lemlib.moveToPoint(30, -48, 1200, {.forwards = false, .minSpeed = 40, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 30);
    delay(200);
    score();
    c_danielib.stopMovement();


    // /* ---------------------------------------------------------------------------------------------- */
    // /*                                    PART 3: MATCHLOAD REPEAT                                    */
    // /* ---------------------------------------------------------------------------------------------- */


    // // matchload sequence
    // c_lemlib.turnToHeading(90, 200);
    // c_lemlib.moveToPoint(2.52_tiles, -2_tiles, 1500);
    // delay(200);
    // load();
    // c_lemlib.waitUntilDone();
    // c_danielib.turnToHeading(0, 600);
    // lemlibDistReset({&right_beam});
    // c_lemlib.moveToPoint(2.49_tiles, -2.8_tiles, 1500, {.forwards = false, .maxSpeed = 50});
    // c_lemlib.waitUntilDone();
    // lemlibDistReset({&right_beam, &back_beam});
    // c_danielib.driveForDistance(1, 150);
    // grab();


    // // scoring sequence
    // c_lemlib.moveToPoint(2.4_tiles, -1.85_tiles, 1500, {.minSpeed = 15, .earlyExitRange = 0});
    // delay(100);
    // a4();
    // c_danielib.turnToHeading(90, 600);
    // c_lemlib.moveToPoint(26, -47, 2300, {.forwards = false, .maxSpeed = 100});
    // delay(2000);
    // score();
    
    
    // /* ---------------------------------------------------------------------------------------------- */
    // /*                                    PART 3: MATCHLOAD REPEAT                                    */
    // /* ---------------------------------------------------------------------------------------------- */


    // // matchload sequence
    // c_lemlib.turnToHeading(90, 200);
    // c_lemlib.moveToPoint(2.52_tiles, -2_tiles, 1500);
    // delay(200);
    // load();
    // c_lemlib.waitUntilDone();
    // c_danielib.turnToHeading(0, 600);
    // lemlibDistReset({&right_beam});
    // c_lemlib.moveToPoint(2.49_tiles, -2.8_tiles, 1500, {.forwards = false, .maxSpeed = 50});
    // c_lemlib.waitUntilDone();
    // lemlibDistReset({&right_beam, &back_beam});
    // c_danielib.driveForDistance(1, 150);
    // grab();


    // // scoring sequence
    // c_lemlib.moveToPoint(2.4_tiles, -1.85_tiles, 1500, {.minSpeed = 15, .earlyExitRange = 0});
    // delay(100);
    // a5();
    // c_danielib.turnToHeading(90, 600);
    // c_lemlib.moveToPoint(26, -47, 2300, {.forwards = false, .maxSpeed = 100});
    // delay(2000);
    // score();
    




    c_lemlib.waitUntilDone();
}

