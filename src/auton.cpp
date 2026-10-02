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


void auton_intake_4pin_far() {
    c_danielib.setPose(6.7, -61.8, 0);
    c_lemlib.setPose(6.7, -61.8, 0);


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
    cupTask();
    c_danielib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(0.5, -28, 1500, {.minSpeed = 10, .earlyExitRange = 10});
    delay(180);
    inPinPosition = true;
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_lemlib.moveToPoint(0, -22.5, 1000, {.maxSpeed = 55});
    delay(10);
    setLiftTo(20);
    intake.brake();
    delay(100);
    intake.move(127);
    delay(500);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    inPinPosition = false;
    // c_lemlib.turnToHeading(20, 100);

    // back up to goal
    c_lemlib.moveToPoint(23, -45, 1400, {.forwards = false, .maxSpeed = 100});
    delay(350);

    // grouping
    setWristTo(0);
    delay(320);
    n1();

    c_lemlib.waitUntilDone();

    // SCIRE DOWN ON NEUTRAL
    score();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // move OFF NEUTRAL and TO FLOWER
    c_lemlib.moveToPoint(5, -28, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(22);
    setWristTo(-11);
    c_lemlib.turnToHeading(90, 500);
    c_lemlib.waitUntilDone();
    intakePin();

    // flower pt
    c_lemlib.moveToPoint(14.8, -24, 1000);
    delay(1000);
    c_lemlib.turnToHeading(115, 100);
    c_lemlib.turnToHeading(75, 100);

    // back up back up!!
    c_lemlib.moveToPoint(-22, -48, 1500, {.forwards = false});
    delay(700);
    a0();
    delay(150);

    // score alliance goal
    intake.move(-127);
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
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */


    // setup inside stack movement
    c_lemlib.moveToPoint(-3, -36, 1200, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(-19.5, -27, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 650, 15);
    c_danielib.async().driveForDistance(3.5, 300);

    // pick up stack
    delay(200);
    grabFlat();

    // lift up
    setLiftTo(25);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(0, 350);
    c_lemlib.moveToPoint(-22, -45, 1300, {.forwards = false});
    a1();

    // c_lemlib.waitUntilDone();
    delay(900);
    
    score();


}


void auton_intake_4pin_outside_far() {
    c_danielib.setPose(6.7, -61.8, 0);
    c_lemlib.setPose(6.7, -61.8, 0);


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
    cupTask();
    c_danielib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PART 1: KNOCK OVER CUP                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // knock over and intake
    c_lemlib.moveToPoint(0.5, -28, 1500, {.minSpeed = 10, .earlyExitRange = 10});
    delay(180);
    inPinPosition = true;
    intake.move(127);
    cone.move(127);
    c_lemlib.waitUntilDone();
    c_lemlib.moveToPoint(0, -22.5, 1000, {.maxSpeed = 55});
    delay(10);
    setLiftTo(20);
    intake.brake();
    delay(100);
    intake.move(127);
    delay(500);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    c_lemlib.turnToHeading(20, 100);
    c_lemlib.turnToHeading(-20, 100);
    inPinPosition = false;
    // c_lemlib.turnToHeading(20, 100);

    // back up to goal
    c_lemlib.moveToPoint(23, -45, 1400, {.forwards = false, .maxSpeed = 100});
    delay(350);

    // grouping
    setWristTo(0);
    delay(320);
    n1();

    c_lemlib.waitUntilDone();

    // SCIRE DOWN ON NEUTRAL
    score();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   PART 2: FLOWER TO ALLIANCE                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // move OFF NEUTRAL and TO FLOWER
    c_lemlib.moveToPoint(5, -28, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    delay(250);
    intake.move(127);
    cone.move(127);
    setLiftTo(22);
    setWristTo(-11);
    c_lemlib.turnToHeading(90, 500);
    c_lemlib.waitUntilDone();
    intakePin();

    // flower pt
    c_lemlib.moveToPoint(14.8, -24, 1000);
    delay(1000);
    c_lemlib.turnToHeading(115, 100);
    c_lemlib.turnToHeading(75, 100);

    // back up back up!!
    c_lemlib.moveToPoint(-20, -42.5, 1500, {.forwards = false, .minSpeed = 60, .earlyExitRange = 5});
    delay(800);
    a0();
    c_lemlib.swingToHeading(-10, DriveSide::RIGHT, 400);
    delay(50);

    // score alliance goal
    intake.move(-127);
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
    /*                                      PART 3: YELLOW STACK                                      */
    /* ---------------------------------------------------------------------------------------------- */


    // c_danielib.driveForDistance(12, 200);
    c_lemlib.turnToHeading(-20, 120);
    c_lemlib.moveToPoint(-28, -36, 300, {.minSpeed = 50, .earlyExitRange = 4});
    c_lemlib.turnToHeading(70, 350);
    hover();
    c_lemlib.moveToPoint(-44.3, -45.1, 1500, {.forwards = false, .maxSpeed = 105, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 500, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(150);
    grabFlat();



    
    // lift up
    setLiftTo(30);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(-80, 310);
    c_lemlib.moveToPoint(-28, -48, 1300, {.forwards = false});
    a2();
    delay(850);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.brake();


}

void auton_4pin_3stack_close() {
    c_danielib.setPose(6.7, -61.8, 0);
    c_lemlib.setPose(6.7, -61.8, 0);


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
    c_lemlib.moveToPoint(3, -36, 1200, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(-110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(19.5, -27, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 650, 15);
    c_danielib.async().driveForDistance(3.5, 300);

    // pick up stack
    delay(200);
    grabFlat();

    // lift up
    setLiftTo(25);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(0, 350);
    c_lemlib.moveToPoint(22, -45, 1300, {.forwards = false});
    a1();
    setWristTo(111);
    delay(900);
    
    // custom slowscore
    lift_has_pid_control = false;
    cone.move(127);
    lift.move(-30);
    delay(600);
    lift_has_pid_control = true;
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 25);
    delay(300);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(12, 250);
    c_lemlib.moveToPoint(24, -24, 1500, {.minSpeed = 25, .earlyExitRange = 6});
    c_lemlib.turnToHeading(-48, 330);
    hover();
    c_lemlib.moveToPoint(44.3, -43.8, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 900, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(200);
    grabFlat();



    
    // lift up
    setLiftTo(30);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -48, 1300, {.forwards = false});
    a2();
    delay(1000);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.brake();
}

void auton_4pin_3stack_far() {
    c_danielib.setPose(-6.7, -61.8, 0);
    c_lemlib.setPose(-6.7, -61.8, 0);


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
    c_lemlib.turnToHeading(90, 400);
    c_lemlib.moveToPoint(-20, -2_tiles, 1300, {.forwards = false});
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
    c_lemlib.moveToPoint(-3, -36, 1200, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(-19.5, -27, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 650, 15);
    c_danielib.async().driveForDistance(3.5, 300);

    // pick up stack
    delay(200);
    grabFlat();

    // lift up
    setLiftTo(25);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(0, 350);
    c_lemlib.moveToPoint(-22, -45, 1300, {.forwards = false});
    a1();
    setWristTo(111);
    delay(900);
    
    // custom slowscore
    lift_has_pid_control = false;
    cone.move(127);
    lift.move(-30);
    delay(600);
    lift_has_pid_control = true;
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 25);
    delay(300);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(12, 250);
    c_lemlib.moveToPoint(-24, -24, 1500, {.minSpeed = 25, .earlyExitRange = 6});
    c_lemlib.turnToHeading(48, 330);
    hover();
    c_lemlib.moveToPoint(-44.3, -43.8, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 900, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(200);
    grabFlat();



    
    // lift up
    setLiftTo(30);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(-80, 400);
    c_lemlib.moveToPoint(-28, -48, 1300, {.forwards = false});
    a2();
    delay(1000);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.brake();
}




// SLOW SKILLS
void auton_skills() {
    c_danielib.setPose(6.7, -61.8, 0);
    c_lemlib.setPose(6.7, -61.8, 0);


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
    c_lemlib.turnToHeading(-85, 300);
    c_lemlib.moveToPoint(20, -2_tiles, 1000, {.forwards = false});
    delay(300);
    flipOut();
    delay(200);
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
    c_lemlib.moveToPoint(3, -36, 1200, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(-110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(19.5, -27, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 650, 15);
    c_danielib.async().driveForDistance(3.5, 300);

    // pick up stack
    delay(200);
    grabFlat();

    // lift up
    setLiftTo(25);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(0, 350);
    c_lemlib.moveToPoint(22, -45, 1300, {.forwards = false});
    a1();
    setWristTo(111);
    delay(900);
    
    // custom slowscore
    lift_has_pid_control = false;
    cone.move(127);
    lift.move(-30);
    delay(600);
    lift_has_pid_control = true;
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 25);
    delay(300);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(12, 250);
    c_lemlib.moveToPoint(24, -24, 1500, {.minSpeed = 25, .earlyExitRange = 6});
    c_lemlib.turnToHeading(-48, 330);
    hover();
    c_lemlib.moveToPoint(44.3, -43.8, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 900, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(200);
    grabFlat();



    
    // lift up
    setLiftTo(30);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -48, 1300, {.forwards = false});
    a2();
    delay(1000);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.brake();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 1: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(2.44_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_danielib.async().turnToHeading(0, 800);
    delay(800);
    lemlibDistReset({&right_beam});

    // back into loader slow
    c_lemlib.moveToPoint(59.1, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(59.2, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 1: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    a3();
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 2: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_lemlib.turnToHeading(0, 500);

    // back into loader slow
    c_lemlib.moveToPoint(59.1, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(59.2, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 2: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    a4();
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();

    /* ---------------------------------------------------------------------------------------------- */
    /*                           SECTION 2: DRIVE LEFT, GRAB PIN FROM FLOWER                          */
    /* ---------------------------------------------------------------------------------------------- */

    // move out of stack
    c_lemlib.moveToPoint(1.75_tiles, -1.7_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.turnToHeading(-80, 400);
    intakePin();
    intake.move(127);
    cone.move(127);

    // move to left flower
    c_lemlib.moveToPoint(0, -33, 2000, {.minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.moveToPoint(-14, -23, 3000);
    c_lemlib.turnToHeading(-30, 100);
    c_lemlib.turnToHeading(-50, 100);
    c_lemlib.turnToHeading(-30, 100);
    c_lemlib.turnToHeading(-50, 100);

    // score pin in neutral
    c_lemlib.turnToHeading(15, 300);
    c_lemlib.moveToPoint(-23, -44, 1500, {.forwards = false});
    delay(350);
    n0();
    c_lemlib.waitUntilDone();

    // score
    score();
    intake.brake();

    /* ---------------------------------------------------------------------------------------------- */
    /*                              PART 2: SWEEP OUT FLOWER AROUND GOAL                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swerve out flower
    c_danielib.driveForDistance(12, 400);
    // c_lemlib.moveToPoint(-1.5_tiles, -1.6_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(-50, 400);
    load();
    intake.move(-127);


    // SWEEP AND SWING
    c_lemlib.moveToPoint(-10.5, -1.9_tiles, 1500, {.forwards = false});
    c_lemlib.swingToHeading(75, DriveSide::LEFT, 600);
    c_lemlib.moveToPoint(-1.6_tiles, -2.35_tiles, 2000, {.forwards = false, .minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.moveToPoint(-2.59_tiles, -1.2_tiles, 2000, {.forwards = false});

    c_lemlib.waitUntilDone();

    

    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 1: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(4, 300);
    c_danielib.async().turnToHeading(0, 1000);
    delay(1000);
    lemlibDistReset({&left_beam});

    // back into loader slow
    c_lemlib.moveToPoint(-59, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 6.5});
    c_lemlib.moveToPoint(-59, -75, 400, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &left_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 1: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    n1();
    c_lemlib.turnToHeading(-80, 400);
    c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.move(-127);


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 2: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(-2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_lemlib.turnToHeading(0, 500);

    // back into loader slow
    c_lemlib.moveToPoint(-58.6, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(-58.7, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &left_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 2: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    n2();
    c_lemlib.turnToHeading(-80, 400);
    c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();

    
    // /* ---------------------------------------------------------------------------------------------- */
    // /*                              MATCHLOAD 3: SETUP + BACK INTO LOADER                             */
    // /* ---------------------------------------------------------------------------------------------- */


    // // set up and line up
    // c_danielib.driveForDistance(16, 200);
    // c_lemlib.moveToPoint(-2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    // load();
    // c_lemlib.waitUntilDone();
    // c_lemlib.turnToHeading(0, 500);

    // // back into loader slow
    // c_lemlib.moveToPoint(-58.6, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    // c_lemlib.moveToPoint(-58.7, -75, 300, {.forwards = false, .maxSpeed = 40});
    // c_lemlib.waitUntilDone();
    // lemlibDistReset({&back_beam, &left_beam});
    // c_danielib.driveForDistance(1.5, 150);
    // grab();


    // /* ---------------------------------------------------------------------------------------------- */
    // /*                                   MATCHLOAD 3: SCORE ON GOAL                                   */
    // /* ---------------------------------------------------------------------------------------------- */


    // // return to goal
    // c_danielib.driveForDistance(16, 350);
    // c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    // n3();
    // c_lemlib.turnToHeading(-80, 400);
    // c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    // c_lemlib.waitUntilDone();
    // c_danielib.async().driveForDistance(-10, 1000, 20);
    // delay(200);
    
    // // lower stack, score, lift off alliance
    // score();
    // c_danielib.stopMovement();








    /* ---------------------------------------------------------------------------------------------- */
    /*                                              PARK                                              */
    /* ---------------------------------------------------------------------------------------------- */

    intake.move(-127);
    c_danielib.driveForDistance(20, 300);
    intakePin();
    c_lemlib.turnToHeading(-135, 250);
    c_lemlib.moveToPoint(-5, -5, 2000, {.forwards = false});

    c_lemlib.waitUntilDone();
    
}



// WORKING SKILLS!!!
void auton_skills_fast() {
    c_danielib.setPose(6.7, -61.8, 0);
    c_lemlib.setPose(6.7, -61.8, 0);


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
    c_lemlib.turnToHeading(-85, 300);
    c_lemlib.moveToPoint(20, -2_tiles, 1000, {.forwards = false});
    delay(300);
    flipOut();
    delay(160);
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
    c_lemlib.moveToPoint(3, -36, 1200, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(-110, 350);
    hover();

    // move to inside stack backwards
    c_lemlib.moveToPoint(19.5, -27, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with inside stack cup
    c_danielib.driveForDistance(-10, 650, 15);
    c_danielib.async().driveForDistance(3.5, 300);

    // pick up stack
    delay(200);
    grabFlat();

    // lift up
    setLiftTo(25);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(0, 350);
    c_lemlib.moveToPoint(22, -42, 1300, {.forwards = false});
    a1();
    setWristTo(111);
    delay(500);
    
    // custom slowscore
    lift_has_pid_control = false;
    cone.move(127);
    lift.move(-30);
    delay(600);
    lift_has_pid_control = true;
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 25);
    delay(300);


    /* ---------------------------------------------------------------------------------------------- */
    /*                                      PART 3: OUTSIDE STACK                                     */
    /* ---------------------------------------------------------------------------------------------- */


    // wiggle around goal to outside yellow stack (COULD BE BETTER)
    c_danielib.driveForDistance(12, 250);
    c_lemlib.moveToPoint(24, -24, 1500, {.minSpeed = 25, .earlyExitRange = 6});
    c_lemlib.turnToHeading(-48, 330);
    hover();
    c_lemlib.moveToPoint(44.3, -43.8, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 7});
    c_lemlib.waitUntilDone();

    // align with stack cup
    c_danielib.driveForDistance(-10, 900, 16);
    c_danielib.async().driveForDistance(3, 300);

    // pick up stack
    delay(200);
    grabFlat();



    
    // lift up
    setLiftTo(30);
    setWristTo(125);

    // score
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -48, 1300, {.forwards = false});
    a2();
    delay(1000);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.brake();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 1: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(2.435_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_lemlib.turnToHeading(0, 600);
    lemlibDistReset({&right_beam});

    // back into loader slow
    c_lemlib.moveToPoint(60.5, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(60.6, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 1: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    a3();
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 2: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_lemlib.turnToHeading(0, 500);

    // back into loader slow
    c_lemlib.moveToPoint(59.1, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(59.2, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &right_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 2: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    a4();
    c_lemlib.turnToHeading(80, 400);
    c_lemlib.moveToPoint(28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();

    /* ---------------------------------------------------------------------------------------------- */
    /*                           SECTION 2: DRIVE LEFT, GRAB PIN FROM FLOWER                          */
    /* ---------------------------------------------------------------------------------------------- */

    // move out of stack
    c_lemlib.moveToPoint(1.75_tiles, -1.7_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.turnToHeading(-80, 400);
    intakePin();
    intake.move(127);
    cone.move(127);

    // move to left flower
    c_lemlib.moveToPoint(0, -33, 2000, {.minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.moveToPoint(-14, -23, 3000);
    c_lemlib.turnToHeading(-30, 100);
    c_lemlib.turnToHeading(-50, 100);
    c_lemlib.turnToHeading(-30, 100);
    c_lemlib.turnToHeading(-50, 100);

    // score pin in neutral
    c_lemlib.turnToHeading(15, 300);
    c_lemlib.moveToPoint(-23, -44, 1500, {.forwards = false});
    delay(350);
    n0();
    c_lemlib.waitUntilDone();

    // score
    score();
    intake.brake();

    /* ---------------------------------------------------------------------------------------------- */
    /*                              PART 2: SWEEP OUT FLOWER AROUND GOAL                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swerve out flower
    c_danielib.driveForDistance(12, 400);
    // c_lemlib.moveToPoint(-1.5_tiles, -1.6_tiles, 1000, {.minSpeed = 20, .earlyExitRange = 5.5});
    c_lemlib.turnToHeading(-50, 400);
    load();
    intake.move(-127);


    // SWEEP AND SWING
    c_lemlib.moveToPoint(-10.5, -1.9_tiles, 1500, {.forwards = false});
    c_lemlib.swingToHeading(75, DriveSide::LEFT, 600);
    c_lemlib.moveToPoint(-1.6_tiles, -2.35_tiles, 2000, {.forwards = false, .minSpeed = 20, .earlyExitRange = 3});
    c_lemlib.moveToPoint(-2.59_tiles, -1.2_tiles, 2000, {.forwards = false});

    c_lemlib.waitUntilDone();

    

    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 1: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(4, 300);
    c_danielib.async().turnToHeading(0, 1000);
    delay(1000);
    lemlibDistReset({&left_beam});

    // back into loader slow
    c_lemlib.moveToPoint(-59, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 6.5});
    c_lemlib.moveToPoint(-59, -75, 400, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &left_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 1: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    n1();
    c_lemlib.turnToHeading(-80, 400);
    c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();
    intake.move(-127);


    /* ---------------------------------------------------------------------------------------------- */
    /*                              MATCHLOAD 2: SETUP + BACK INTO LOADER                             */
    /* ---------------------------------------------------------------------------------------------- */


    // set up and line up
    c_danielib.driveForDistance(16, 200);
    c_lemlib.moveToPoint(-2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    load();
    c_lemlib.waitUntilDone();
    c_lemlib.turnToHeading(0, 500);

    // back into loader slow
    c_lemlib.moveToPoint(-58.6, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    c_lemlib.moveToPoint(-58.7, -75, 300, {.forwards = false, .maxSpeed = 40});
    c_lemlib.waitUntilDone();
    lemlibDistReset({&back_beam, &left_beam});
    c_danielib.driveForDistance(1.5, 150);
    grab();


    /* ---------------------------------------------------------------------------------------------- */
    /*                                   MATCHLOAD 2: SCORE ON GOAL                                   */
    /* ---------------------------------------------------------------------------------------------- */


    // return to goal
    c_danielib.driveForDistance(16, 350);
    c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    n2();
    c_lemlib.turnToHeading(-80, 400);
    c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    c_lemlib.waitUntilDone();
    c_danielib.async().driveForDistance(-10, 1000, 20);
    delay(200);
    
    // lower stack, score, lift off alliance
    score();
    c_danielib.stopMovement();

    
    // /* ---------------------------------------------------------------------------------------------- */
    // /*                              MATCHLOAD 3: SETUP + BACK INTO LOADER                             */
    // /* ---------------------------------------------------------------------------------------------- */


    // // set up and line up
    // c_danielib.driveForDistance(16, 200);
    // c_lemlib.moveToPoint(-2.42_tiles, -1.6_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    // load();
    // c_lemlib.waitUntilDone();
    // c_lemlib.turnToHeading(0, 500);

    // // back into loader slow
    // c_lemlib.moveToPoint(-58.6, -61, 2000, {.forwards = false, .maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 5});
    // c_lemlib.moveToPoint(-58.7, -75, 300, {.forwards = false, .maxSpeed = 40});
    // c_lemlib.waitUntilDone();
    // lemlibDistReset({&back_beam, &left_beam});
    // c_danielib.driveForDistance(1.5, 150);
    // grab();


    // /* ---------------------------------------------------------------------------------------------- */
    // /*                                   MATCHLOAD 3: SCORE ON GOAL                                   */
    // /* ---------------------------------------------------------------------------------------------- */


    // // return to goal
    // c_danielib.driveForDistance(16, 350);
    // c_lemlib.moveToPoint(-2.25_tiles, -2_tiles, 1500, {.minSpeed = 20, .earlyExitRange = 4});
    // n3();
    // c_lemlib.turnToHeading(-80, 400);
    // c_lemlib.moveToPoint(-28, -47.9, 1500, {.forwards = false, .minSpeed = 25, .earlyExitRange = 4});
    // c_lemlib.waitUntilDone();
    // c_danielib.async().driveForDistance(-10, 1000, 20);
    // delay(200);
    
    // // lower stack, score, lift off alliance
    // score();
    // c_danielib.stopMovement();








    /* ---------------------------------------------------------------------------------------------- */
    /*                                              PARK                                              */
    /* ---------------------------------------------------------------------------------------------- */

    intake.move(-127);
    c_danielib.driveForDistance(20, 300);
    intakePin();
    c_lemlib.turnToHeading(-135, 250);
    c_lemlib.moveToPoint(-5, -5, 2000, {.forwards = false});

    c_lemlib.waitUntilDone();
    
}
