#include "main.h"
using namespace pros;

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

constexpr double operator""_tiles(long double value) {
    return value * 23.622;
}
constexpr double operator""_tiles(unsigned long long value) {
    return static_cast<double>(value) * 23.622;
}
void store() {
    hood.retract();
    bottom.move(127);
    top.move(127);
}
void score() {
    hood.extend();
    bottom.move(127);
    top.move(127);
}
void score_mid() {
    hood.retract();
    bottom.move(127);
    top.move(-127);
}
void stop() {
    bottom.brake();
    top.brake();
}
void outtake() {
    hood.retract();
    bottom.move(-127);
    top.move(-127);
}

void auton_none() {
    chassis.setPose(0, 0, 0);

    chassis.turnToHeading(75, 100000, {}, false);

    // 10 - 6.4
    // 20 - 18.5
    // 30 - 28.3
    // 40 - 38.7
    // 60 - 59.2
    // 75 - 75.1
    // 90 - 
}

// /* ---------------------------------------------------------------------------------------------- */
// /*                                        NONE (TEST AUTO)                                        */
// /* ---------------------------------------------------------------------------------------------- */

// // drive off line
// void auton_none() {
//     chassis.setPose(0, 0, 0);

//     chassis.driveForDistance(2, 1000);
// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                              SAWPS                                             */
// /* ---------------------------------------------------------------------------------------------- */

// // PUSH COUNTER SAWP WING
// void auton_sawp_counter_wing() {
//     int startTime = millis();
//     chassis.setPose(2, -47.5, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              PUSH                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // push and steal preload
//     store();
//     chassis.driveForDistance(7, 350);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                          RIGHT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.88_tiles, -1.92_tiles, 1200, true);
//     delay(600);
//     loader.extend();
//     chassis.turnToHeading(180, 500);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 38);
//     waitUntilCondition(millis() >= loader1Start + 740);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      RIGHT LONG GOAL SCORE                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2.03_tiles, -24, 1300, true, 95);
//     delay(900);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 700);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           MID STACKS                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal and drive to stack
//     loader.retract();
//     stop();
//     hood.extend();
//     chassis.turnToHeading(271, 500);
//     score_mid();
//     chassis.async().moveToPoint(0.4_tiles, -0.99_tiles, 1400, false, 100, 8);
//     delay(400);
//     store();
//     chassis.waitUntilDone();

//     // drive to left side stack
//     chassis.async().moveToPoint(-0.88_tiles, -1.02_tiles, 1400, false, 90, 7);
//     delay(380);
//     loader.extend();
//     // chassis.turnToHeading(225, 250);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      LEFT LONG GOAL SCORE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive to goal setup
//     chassis.moveToPoint(-1.85_tiles, -1.78_tiles, 900, false, 80);
//     chassis.async().turnToHeading(180, 420);
//     outtake();
//     delay(100);
//     store();
//     chassis.waitUntilDone();

//     // distance reset
//     chassis.distanceResetPose({&right_beam});

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 800, true);
//     delay(650);
//     score();
//     int score2Start = millis();
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score2Start + 670);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);
//     // chassis.distanceResetPose({&right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     hood.extend();
//     top.brake();
//     bottom.move(-127);
//     chassis.async().moveToPoint(-2_tiles, -53, 1300, false, 90, 7);
//     delay(200);
//     store();
//     chassis.waitUntilDone();
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-2_tiles, -70, 1000, false, 30);
//     waitUntilCondition(millis() >= loader2Start + 930);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         MID GOAL SCORE                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to mid goal and score
//     chassis.driveForDistance(-11, 700, 100, 8);
//     chassis.async().moveToPoint(-15, -14, 1700, true, 100, 8);
//     delay(400);
//     bottom.move(-127);
//     delay(100);
//     stop();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(-8, -8, 1000, true, 70);
//     delay(200);

//     // start scoring
//     int midScoreStart = millis();
//     score_mid();

//     // wait until done
//     waitUntilCondition(millis() >= midScoreStart + 1000 || millis() >= startTime + 13300);
//     chassis.stopMovement();
//     loader.retract();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive for wing
//     stop();
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);
//     chassis.moveToPoint(-1.45_tiles, -1.55_tiles, 1400, false, 100, 6);
//     // chassis.turnToHeading(180, 400);

//     // wing push
//     chassis.moveToPoint(-1.4_tiles, -9, 1200, true, 100);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(190, danielib::SwingSide::LEFT, 600);

// }

// // PUSH COUNTER SAWP NO WING
// void auton_sawp_counter_nowing() {
//     int startTime = millis();
//     chassis.setPose(2, -47.5, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              PUSH                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // push and steal preload
//     store();
//     chassis.driveForDistance(7, 350);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                          RIGHT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.88_tiles, -1.92_tiles, 1200, true);
//     delay(600);
//     loader.extend();
//     chassis.turnToHeading(180, 500);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 38);
//     waitUntilCondition(millis() >= loader1Start + 740);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      RIGHT LONG GOAL SCORE                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2.03_tiles, -24, 1300, true, 95);
//     delay(900);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 700);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           MID STACKS                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal and drive to stack
//     loader.retract();
//     stop();
//     hood.extend();
//     chassis.turnToHeading(271, 500);
//     score_mid();
//     chassis.async().moveToPoint(0.4_tiles, -0.99_tiles, 1400, false, 100, 8);
//     delay(400);
//     store();
//     chassis.waitUntilDone();

//     // drive to left side stack
//     chassis.async().moveToPoint(-0.88_tiles, -1.02_tiles, 1400, false, 90, 7);
//     delay(380);
//     loader.extend();
//     // chassis.turnToHeading(225, 250);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      LEFT LONG GOAL SCORE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive to goal setup
//     chassis.moveToPoint(-1.85_tiles, -1.78_tiles, 900, false, 80);
//     chassis.async().turnToHeading(180, 420);
//     outtake();
//     delay(100);
//     store();
//     chassis.waitUntilDone();

//     // distance reset
//     chassis.distanceResetPose({&right_beam});

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 800, true);
//     delay(650);
//     score();
//     int score2Start = millis();
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score2Start + 630);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);
//     // chassis.distanceResetPose({&right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     hood.extend();
//     top.brake();
//     bottom.move(-127);
//     chassis.async().moveToPoint(-2_tiles, -53, 1300, false, 90, 7);
//     delay(200);
//     store();
//     chassis.waitUntilDone();
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-2_tiles, -70, 1000, false, 30);
//     waitUntilCondition(millis() >= loader2Start + 910);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         MID GOAL SCORE                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to mid goal and score
//     chassis.driveForDistance(-11, 700, 100, 8);
//     chassis.async().moveToPoint(-15, -14, 1700, true, 100, 8);
//     delay(400);
//     bottom.move(-127);
//     delay(100);
//     stop();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(-8, -8, 400, true, 40);
//     delay(270);

//     // start scoring
//     int midScoreStart = millis();
//     bottom.move(80);
//     top.move(-75);
// }

// // PUSH STANDARD SAWP
// void auton_sawp_standard() {
//     int startTime = millis();
//     chassis.setPose(2, -47.5, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              PUSH                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // push and steal preload
//     store();
//     chassis.driveForDistance(7, 350);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                          RIGHT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.88_tiles, -1.92_tiles, 1200, true);
//     delay(600);
//     loader.extend();
//     chassis.turnToHeading(180, 500);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 38);
//     waitUntilCondition(millis() >= loader1Start + 740);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      RIGHT LONG GOAL SCORE                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2.03_tiles, -24, 1300, true, 95);
//     delay(900);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 700);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           MID STACKS                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal and drive to stack
//     loader.retract();
//     chassis.turnToHeading(271, 500);
//     store();
//     chassis.moveToPoint(0.4_tiles, -0.99_tiles, 1400, false, 100, 8);

//     // drive to left side stack
//     chassis.async().moveToPoint(-0.9_tiles, -1.01_tiles, 1200, false, 90);
//     delay(400);
//     loader.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            MID GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to mid goal
//     chassis.async().moveToPoint(-8, -8, 1200, true, 60);
//     delay(500);
//     outtake();
//     delay(200);
//     stop();
//     delay(500);

//     // score mid goal
//     int midScoreStart = millis();
//     score_mid();

//     waitUntilCondition(millis() >= midScoreStart + 1000);
//     stop();
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // move to loader
//     chassis.async().moveToPoint(-1.88_tiles, -1.85_tiles, 1500, false, 85);
//     top.move(127);
//     bottom.move(-80);
//     delay(150);
//     bottom.brake();
//     delay(200);
//     store();
//     chassis.waitUntilDone();
//     loader.extend();
//     chassis.turnToHeading(180, 400);
//     chassis.distanceResetPose({&right_beam});

//     // drive into loader
//     mid_descore.retract();
//     chassis.moveToPoint(-1.95_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-1.95_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader2Start + 900);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      LEFT LONG GOAL SCORE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 1300, true, 85);
//     delay(950);
//     score();
//     int score2Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // // wait until done
//     // waitUntilCondition(millis() >= score2Start + 600);
//     // chassis.stopMovement();

//     // // distance reset
//     // chassis.setPose(-2_tiles, -28);
// }

// // PUSH LOW GOAL SAWP
// void auton_sawp_low_mid() {
//     int startTime = millis();
//     chassis.setPose(2, -47.5, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              PUSH                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // push and steal preload
//     store();
//     chassis.driveForDistance(7, 350);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                          RIGHT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.88_tiles, -1.92_tiles, 1200, true);
//     delay(600);
//     loader.extend();
//     chassis.turnToHeading(180, 500);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 35);
//     waitUntilCondition(millis() >= loader1Start + 740);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      RIGHT LONG GOAL SCORE                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2.03_tiles, -24, 1300, true, 95);
//     delay(900);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 700);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                   RIGHT SIDE STACK + LOW GOAL                                  */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal and drive to stack
//     loader.retract();
//     chassis.turnToHeading(271, 500);
//     store();
//     chassis.async().moveToPoint(0.83_tiles, -1.03_tiles, 800, false, 80);
//     delay(450);
//     loader.extend();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(325, 400);

//     // move to low goal and score
//     mid_descore.extend();
//     loader.retract();
//     chassis.async().moveToPoint(12, -13.5, 1000, 80);
//     delay(500);
//     int lowScoreStart = millis();
//     outtake();
//     bottom.move(-105);
//     chassis.async().turnToHeading(315, 300);

//     // wait until done
//     waitUntilCondition(millis() >= lowScoreStart + 900);
//     chassis.stopMovement();

//     // back out
//     chassis.moveToPoint(1_tiles, -0.89_tiles, 900, true, 100, 1);
//     store();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LEFT SIDE STACK                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive to left side stack
//     chassis.async().moveToPoint(-0.88_tiles, -0.93_tiles, 1700, false, 90, 7);
//     delay(900);
//     loader.extend();

//     // drive to loader setup
//     chassis.moveToPoint(-1.88_tiles, -1.85_tiles, 1400, false, 80);
//     chassis.async().turnToHeading(180, 420);
//     outtake();
//     delay(100);
//     store();
//     chassis.waitUntilDone();

//     // distance reset
//     chassis.distanceResetPose({&right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive into loader
//     mid_descore.retract();
//     chassis.moveToPoint(-1.95_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-1.95_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader2Start + 900);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      LEFT LONG GOAL SCORE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 1300, true, 85);
//     delay(950);
//     score();
//     int score2Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 300, 60);
//     chassis.async().driveForDistance(-6, 1000, 40);

//     waitUntilCondition(millis() >= score2Start + 900);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive for wing
//     loader.retract();
//     chassis.moveToPoint(-1.61_tiles, -1.52_tiles, 900, false, 100, 4);
//     chassis.turnToHeading(190, 400);

//     // wing push
//     chassis.moveToPoint(-1.5_tiles, -1_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-1.5_tiles, -9, 1000, true, 100);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(190, danielib::SwingSide::LEFT, 600);

// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                             SPLITS                                             */
// /* ---------------------------------------------------------------------------------------------- */

// // LEFT SIDE SPLIT - stack? SETUP
// void auton_left_split() {
//     chassis.setPose(-14.3, -48, 0);
//     store();
//     mid_descore.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(-1.06_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // swing to long
//     // chassis.moveToPoint(-1.8_tiles, -1.28_tiles, 1000, true, 100, 8);
//     // chassis.turnToHeading(135, 500);
//     // chassis.swingToHeading(180, danielib::SwingSide::RIGHT, 800);
//     chassis.moveToPoint(-1.37_tiles, -1.5_tiles, 1000, true, 100, 8);
//     chassis.swingToHeading(155, danielib::SwingSide::RIGHT, 650);
//     chassis.moveToPoint(-2_tiles, -26, 900, true);

//     score();
//     int score2Start = millis();
//     chassis.waitUntilDone();

//     // wait until done
//     waitUntilCondition(millis() >= score2Start + 550);
//     chassis.stopMovement();

//     // realign
//     chassis.turnToHeading(180, 300, 60);
//     loader.extend();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT LOADER                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     hood.extend();
//     top.brake();
//     bottom.move(-127);
//     chassis.async().moveToPoint(-1.97_tiles, -53, 1300, false, 90, 7);
//     delay(200);
//     store();
//     chassis.waitUntilDone();
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-1.97_tiles, -70, 1000, false, 30);
//     waitUntilCondition(millis() >= loader2Start + 930);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         MID GOAL SCORE                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to mid goal and score
//     chassis.driveForDistance(-11, 700, 100, 8);
//     mid_descore.retract();
//     chassis.async().moveToPoint(-15, -14, 1700, true, 100, 8);
//     delay(400);
//     bottom.move(-127);
//     delay(100);
//     stop();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(-8, -8, 1000, true, 70);
//     delay(200);

//     // start scoring
//     int midScoreStart = millis();
//     score_mid();

//     // wait until done
//     waitUntilCondition(millis() >= midScoreStart + 900);
//     chassis.stopMovement();
//     loader.retract();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive for wing
//     stop();
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);
//     chassis.moveToPoint(-1.4_tiles, -1.55_tiles, 1400, false, 100, 6);
//     // chassis.turnToHeading(180, 400);

//     // wing push
//     chassis.moveToPoint(-1.39_tiles, -12, 1500, true, 100);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(190, danielib::SwingSide::LEFT, 600);

// }

// // RIGHT SIDE SPLIT - LOADER SETUP
// void auton_right_split() {
//     int startTime = millis();
//     chassis.setPose(17.5, -49, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.9_tiles, -1.92_tiles, 900, true, 90);
//     delay(200);
//     loader.extend();
//     wing.extend();
//     mid_descore.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -57, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 35);
//     waitUntilCondition(millis() >= loader1Start + 720);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2_tiles, -23, 1000, true, 90);
//     delay(800);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 510);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                   RIGHT SIDE STACK + LOW GOAL                                  */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal and drive to stack
//     loader.retract();
//     chassis.turnToHeading(279, 200);
//     chassis.async().moveToPoint(0.79_tiles, -1.03_tiles, 1000, false, 80);
//     delay(100);
//     stop();
//     hood.retract();
//     delay(360);
//     loader.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(1.5_tiles, -1.5_tiles, 800, true, 85);
//     delay(100);
//     loader.retract();
//     chassis.turnToHeading(315, 400);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LOW GOAL SCORE                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // move to low goal and score
//     mid_descore.extend();
//     chassis.async().moveToPoint(8, -8, 1200, false, 60);
//     delay(400);
//     outtake();
//     delay(100);
//     store();
//     chassis.waitUntilDone();
//     chassis.driveForDistance(-5.8, 800, 90);
//     int lowScoreStart = millis();
//     delay(100);
//     outtake();
//     bottom.move(-100);

//     // wait until done
//     waitUntilCondition(millis() >= lowScoreStart + 1200);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back for wing
//     stop();
//     chassis.moveToPoint(1.6_tiles, -1.5_tiles, 1200, true, 100, 7);
//     store();
//     mid_descore.retract();
//     wing.retract();
//     chassis.moveToPoint(2.35_tiles, -1.6_tiles, 860, true, 90, 7);

//     // back up fast then slow
//     chassis.moveToPoint(2_tiles+0.57_tiles, -1.22_tiles, 900, true, 100, 7);
//     chassis.moveToPoint(2_tiles+0.46_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.46_tiles, -8.5, 1300, true, 56);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                             4 BALLS                                            */
// /* ---------------------------------------------------------------------------------------------- */

// // LEFT SIDE 4 BALL - LOADER SETUP
// void auton_left_4ball_loader() {
//     chassis.setPose(-17.5, -49, 90);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(-1.9_tiles, -1.92_tiles, 900, true, 90);
//     delay(200);
//     loader.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&right_beam});

//     // drive into loader
//     chassis.moveToPoint(-1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(-1.97_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader1Start + 780);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 1300, true, 95);
//     delay(800);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 630);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(-2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(-2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// // RIGHT SIDE 4 BALL - LOADER SETUP
// void auton_right_4ball_loader() {
//     chassis.setPose(17.5, -49, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.9_tiles, -1.92_tiles, 900, true, 90);
//     delay(200);
//     loader.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader1Start + 780);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2_tiles, -24, 1300, true, 95);
//     delay(800);
//     score();
//     int score1Start = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= score1Start + 630);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// // LEFT SIDE 4 BALL - STACK SETUP
// void auton_left_4ball_stack() {
//     chassis.setPose(-14.3, -48, 0);
//     store();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(-1.02_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // swing to long
//     chassis.moveToPoint(-1.35_tiles, -1.5_tiles, 1000, true, 100, 7);
//     chassis.swingToHeading(148, danielib::SwingSide::RIGHT, 600);
//     chassis.async().moveToPoint(-2_tiles, -26, 900, true);

//     // score
//     delay(800);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 350);
//     chassis.async().driveForDistance(-5, 1000, 50);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 600);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(-2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(-2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// // RIGHT SIDE 4 BALL - STACK SETUP
// void auton_right_4ball_stack() {
//     chassis.setPose(14.3, -48, 0);
//     store();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(1.02_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // swing to long
//     chassis.moveToPoint(1.38_tiles, -1.5_tiles, 1000, true, 100, 7);
//     chassis.swingToHeading(-148, danielib::SwingSide::LEFT, 600);
//     chassis.async().moveToPoint(2_tiles, -26, 900, true);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // score
//     delay(800);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 350);
//     chassis.async().driveForDistance(-5, 1000, 50);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 600);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                             7 BALLS                                            */
// /* ---------------------------------------------------------------------------------------------- */

// // RIGHT SIDE 7 BALL - STACK SETUP
// void auton_right_7ball() {
//     chassis.setPose(14.3, -48, 0);
//     store();
//     mid_descore.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(1.02_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // turn to loader area
//     chassis.turnToHeading(150, 500);
//     chassis.moveToPoint(1.94_tiles, -1.8_tiles, 1000, false, 85);
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -57, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 35);
//     waitUntilCondition(millis() >= loader1Start + 780);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2_tiles, -24, 1300, true, 95);
//     delay(840);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 1200);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     chassis.waitUntilDone();
//     store();

//     // back up fast then slow
//     chassis.moveToPoint(2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// void auton_left_7ball() {
//     chassis.setPose(-14.3, -48, 0);
//     store();
//     mid_descore.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(-1.02_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // turn to loader area
//     chassis.turnToHeading(-150, 500);
//     chassis.moveToPoint(-1.94_tiles, -1.8_tiles, 1000, false, 85);
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive into loader
//     chassis.moveToPoint(-1.97_tiles, -57, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(-1.97_tiles, -70, 1000, false, 35);
//     waitUntilCondition(millis() >= loader1Start + 780);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 1300, true, 95);
//     delay(840);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 1200);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(-2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     chassis.waitUntilDone();
//     store();

//     // back up fast then slow
//     chassis.moveToPoint(-2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -9, 1300, true, 50);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                         7 BALL COUNTERS                                        */
// /* ---------------------------------------------------------------------------------------------- */


// // goated 7ball counter
// void auton_left_7ball_counter() {
//     chassis.setPose(-14.3, -48, 0);
//     store();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // fast grab motion chain
//     chassis.async().moveToPoint(-1.02_tiles, -0.95_tiles, 900, false, 100);
//     delay(550);
//     loader.extend();

//     // turn to loader area
//     chassis.turnToHeading(-150, 500);
//     chassis.moveToPoint(-1.94_tiles, -1.8_tiles, 1000, false, 85);
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive into loader
//     chassis.moveToPoint(-1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(-1.97_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader1Start + 780);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            MID GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to mid goal and score
//     chassis.driveForDistance(-11, 700, 100, 8);
//     chassis.async().moveToPoint(-15, -14, 1700, true, 100, 8);
//     delay(400);
//     bottom.move(-127);
//     delay(100);
//     stop();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(-8, -8, 400, true, 40);
//     delay(270);

//     // start scoring
//     int midScoreStart = millis();
//     score_mid();

//     // wait until done
//     waitUntilCondition(millis() >= midScoreStart + 700);
//     chassis.stopMovement();
//     loader.retract();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         WING SWEEP OUT                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive for wing
//     stop();
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);
//     wing.extend();
//     chassis.async().moveToPoint(-1.57_tiles, -1.7_tiles, 1500, false, 85);
//     bottom.move(-127);
//     top.brake();
//     delay(300);
//     top.move(127);
//     bottom.brake();
//     delay(300);
//     store();
//     chassis.waitUntilDone();

//     // wing sweep
//     chassis.turnToHeading(180, 400);
//     chassis.distanceResetPose({&right_beam});
//     stop();
//     chassis.moveToPoint(-38, -0.5_tiles, 1200, true, 100, 6);
//     chassis.moveToPoint(-37, -11, 1000, true, 80);
//     wing.retract();
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 300);
//     chassis.moveToPoint(chassis.getPose().x, -1.3_tiles, 1300, false, 100, 7);

//     // lineup long goal
//     chassis.moveToPoint(-2.1_tiles, -1.7_tiles, 1000, false, 90);
//     chassis.turnToHeading(180, 400);
//     chassis.distanceResetPose({&right_beam});


//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, -24, 700, true, 75);
//     delay(500);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 1000);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(-2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(-2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(-2_tiles+0.42_tiles, -9, 1300, true, 65);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);
// }

// void auton_right_7ball_counter() {
//     chassis.setPose(17.5, -49, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.9_tiles, -1.92_tiles, 900, true, 90);
//     delay(200);
//     loader.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 40);
//     waitUntilCondition(millis() >= loader1Start + 740);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              STACK                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     chassis.moveToPoint(2_tiles, -1.8_tiles, 700, true);
//     loader.retract();
//     chassis.turnToHeading(-45, 600);
//     store();

//     // SCORE LOW GOAL STUFF
//     chassis.async().moveToPoint(0.9_tiles, -0.9_tiles, 1200, false, 80);
//     delay(700);
//     loader.extend();
//     store();
//     chassis.waitUntilDone();
//     // chassis.moveToPoint(1.5_tiles, -1.5_tiles, 700, true, 85, 3);
//     // chassis.turnToHeading(315, 400);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            LOW GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     // move to low goal and score
//     mid_descore.extend();
//     loader.retract();
//     chassis.moveToPoint(10, -10.5, 1000, false, 50);
//     chassis.turnToHeading(-45, 300);
//     chassis.moveToPoint(15, -14, 600, true);
//     int lowScoreStart = millis();
//     outtake();
//     bottom.move(-100);

//     // wait until done
//     waitUntilCondition(millis() >= lowScoreStart + 750);
//     chassis.stopMovement();


//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         WING SWEEP OUT                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive for wing
//     stop();
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);
//     wing.extend();
//     chassis.async().moveToPoint(2_tiles+0.53_tiles, -1.7_tiles, 1500, false, 85);
//     bottom.move(-127);
//     top.brake();
//     delay(300);
//     top.move(127);
//     bottom.brake();
//     delay(300);
//     store();
//     chassis.waitUntilDone();

//     // wing sweep
//     chassis.turnToHeading(180, 400);
//     chassis.distanceResetPose({&left_beam});
//     stop();
//     chassis.moveToPoint(2_tiles+9.2, -0.5_tiles, 1200, true, 100, 6);
//     chassis.moveToPoint(2_tiles+10, -11, 1000, true, 80);
//     wing.retract();
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 300);
//     chassis.moveToPoint(chassis.getPose().x, -1.3_tiles, 1300, false, 100, 7);

//     // lineup long goal
//     chassis.moveToPoint(1.9_tiles, -1.7_tiles, 1000, false, 90);
//     chassis.turnToHeading(180, 400);
//     chassis.distanceResetPose({&left_beam});


//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LONG GOAL SCORE                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2_tiles, -24, 700, true, 75);
//     delay(500);
//     score();
//     int scoreStart = millis();

//     // realign
//     chassis.waitUntilDone();
//     chassis.async().turnToHeading(180, 250);
//     chassis.async().driveForDistance(-5, 1000, 40);

//     // wait until done
//     waitUntilCondition(millis() >= scoreStart + 1000);
//     chassis.stopMovement();
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     // chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              WING                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // swing out of long goal
//     hood.extend();
//     chassis.async().moveToPoint(2_tiles+0.3_tiles, -1.5_tiles, 1000, false, 100, 3);
//     delay(100);
//     stop();
//     delay(500);
//     hood.retract();
//     chassis.waitUntilDone();

//     // back up fast then slow
//     chassis.moveToPoint(2_tiles+0.4_tiles, -1.25_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -0.8_tiles, 900, true, 100, 6);
//     chassis.moveToPoint(2_tiles+0.42_tiles, -9, 1300, true, 65);

//     // hold and swing
//     left_mg.set_brake_mode_all(MotorBrake::hold);
//     right_mg.set_brake_mode_all(MotorBrake::hold);
//     chassis.swingToHeading(180, danielib::SwingSide::LEFT, 600);


// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                           MISC AUTOS                                           */
// /* ---------------------------------------------------------------------------------------------- */

// void auton_all_mid() {
//     int startTime = millis();
//     chassis.setPose(17.5, -49, 270);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                             LOADER                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive back to loader
//     chassis.async().moveToPoint(1.9_tiles, -1.92_tiles, 900, true, 90);
//     delay(200);
//     loader.extend();
//     wing.extend();
//     mid_descore.extend();
//     store();
//     chassis.waitUntilDone();
//     chassis.turnToHeading(180, 450);

//     // distance reset
//     chassis.distanceResetPose({&left_beam});

//     // drive into loader
//     chassis.moveToPoint(1.97_tiles, -55, 1300, false, 90, 6);
//     store();
//     int loader1Start = millis();
//     chassis.async().moveToPoint(1.97_tiles, -70, 1000, false, 30);
//     waitUntilCondition(millis() >= loader1Start + 690);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              STACK                                             */
//     /* ---------------------------------------------------------------------------------------------- */

//     chassis.moveToPoint(2_tiles, -1.8_tiles, 700, true);
//     loader.retract();
//     chassis.turnToHeading(-45, 600);
//     store();

//     // silly weird thing
//     chassis.async().moveToPoint(0.9_tiles, -0.9_tiles, 3000, false, 35);
//     delay(700);
//     // loader.extend();
//     store();
//     chassis.waitUntilDone();
//     // chassis.moveToPoint(1.5_tiles, -1.5_tiles, 700, true, 85, 3);
//     // chassis.turnToHeading(315, 400);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            LOW GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     // move to low goal and score
//     loader.retract();
//     chassis.turnToHeading(-48, 400);
//     chassis.async().moveToPoint(8, -8, 2000, false, 40);
//     delay(600);
//     outtake();
//     delay(100);
//     store();
//     chassis.waitUntilDone();
//     chassis.driveForDistance(-5.8, 1100, 70);
//     waitUntilCondition(millis() >= startTime + 9000);
//     int lowScoreStart = millis();
//     outtake();
//     bottom.move(-80);
//     delay(600);
//     bottom.move(-110);
//     delay(500);
//     bottom.move(-65);
//     delay(200);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT STACK                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back out
//     store();
//     chassis.moveToPoint(1_tiles, -0.89_tiles, 900, true, 100, 1);

//     // drive to stack
//     mid_descore.retract();
//     chassis.async().moveToPoint(-0.95_tiles, -0.93_tiles, 1700, false, 90);
//     delay(900);
//     loader.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            MID GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     chassis.turnToHeading(-135, 500);
//     chassis.async().moveToPoint(-15, -14, 1000, true, 100, 7);
//     delay(200);
//     bottom.move(-127);
//     delay(100);
//     stop();
//     chassis.waitUntilDone();
//     chassis.async().moveToPoint(-8, -8, 600, true, 40);
//     delay(300);

//     // start scoring
//     int midScoreStart = millis();
//     score_mid();

//     // wait until done
//     waitUntilCondition(millis() >= midScoreStart + 560);
//     chassis.stopMovement();
//     loader.retract();
// }

// /* ---------------------------------------------------------------------------------------------- */
// /*                                             SKILLS                                             */
// /* ---------------------------------------------------------------------------------------------- */

// void auton_skills() {
//     int startTime = millis();
//     drive.setPose(0, -54, 180);
//     odom_lift.extend();
//     store();
//     left_mg.set_brake_mode_all(MotorBrake::coast);
//     right_mg.set_brake_mode_all(MotorBrake::coast);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                       CLEAR RED PARK ZONE                                      */
//     /* ---------------------------------------------------------------------------------------------- */
// }

// void auton_skills_old() {
//     int startTime = millis();
//     chassis.setPose(0, -54, 185);
//     odom_lift.extend();
//     store();
//     left_mg.set_brake_mode_all(MotorBrake::coast);
//     right_mg.set_brake_mode_all(MotorBrake::coast);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                       CLEAR RED PARK ZONE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // start with front wheel in and drive in
//     int parkStart = millis();
//     chassis.driveForDistance(3, 500, 40);

//     // wiggle side to side
//     chassis.turnToHeading(180-14, 130, 85);
//     chassis.turnToHeading(180+14, 130, 85);
//     chassis.turnToHeading(180-14, 130, 85);
//     chassis.turnToHeading(180, 100);

//     // drive fully into wall for last 3 blocks
//     waitUntilCondition(millis() >= parkStart + 1400);
//     chassis.driveForDistance(8, 800);

//     // wiggle back and forth
//     chassis.driveForDistance(4, 100);
//     chassis.async().driveForDistance(-2, 190);

//     // wiggle side to side
//     chassis.turnToHeading(180-14, 130, 85);
//     chassis.turnToHeading(180+14, 130, 85);
//     chassis.turnToHeading(180-14, 130, 85);
//     chassis.turnToHeading(180, 130);

//     // back and forth again
//     chassis.driveForDistance(5, 200);
//     chassis.driveForDistance(-2, 150);
//     chassis.driveForDistance(4, 200);

//     // wait until done
//     waitUntilCondition(millis() >= parkStart + 3300);

//     // back all the way out of park zone
//     // chassis.distanceResetPose({&front_beam});
//     chassis.driveForDistance(-20, 1100);
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                   REALIGN AND DISTANCE RESET                                   */
//     /* ---------------------------------------------------------------------------------------------- */

//     // turn around and back up to reset
//     chassis.distanceResetPose({&front_beam});
//     chassis.turnToHeading(0, 600);
//     odom_lift.retract();
//     chassis.moveToPoint(chassis.getPose().x, -49.2, 700, true, 45);
//     stop();

//     // distance reset
//     chassis.setPose(0, -47.6, 0);
//     chassis.distanceResetPose({&left_beam, &right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      RIGHT LOW GOAL STACK                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // get single block from stack
//     store();
//     chassis.async().moveToPoint(15.5, -29, 1600, false, 65);
//     // delay(150);
//     outtake();
//     delay(120);
//     store();

//     // swing out and drive to low goal
//     chassis.swingToHeading(0, danielib::SwingSide::LEFT, 430);
//     chassis.moveToPoint(15, -21, 800, false, 80, 7.5);
//     stop();
//     intake_raise.extend();
//     mid_descore.extend();
//     chassis.moveToPoint(9, -11.5, 1000, false, 70);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         SCORE LOW GOAL                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // first fast score
//     chassis.async().turnToHeading(-45, 300, 100, false);
//     bottom.move(-62);
//     top.brake();
//     delay(400);

//     // medium score
//     bottom.move(-40);
//     top.move(-30);
//     delay(1700);

//     // slow score end
//     chassis.async().driveForDistance(-1.6, 100);
//     bottom.move(-27);
//     top.brake();
//     delay(1000);

//     // last block anti throw code
//     chassis.async().driveForDistance(-5, 350);
//     delay(400);
//     bottom.move(-50);
//     intake_raise.retract();
//     mid_descore.retract();
//     delay(200);
//     stop();
//     // chassis.driveForDistance(3, 400, 30);

//     // back up and lower intake
//     chassis.driveForDistance(-11, 900, 80);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         LEFT STACK - 1                                         */
//     /* ---------------------------------------------------------------------------------------------- */

//     // turn to stack
//     chassis.turnToHeading(270, 450);
//     score();

//     // drive through left stack and move between goal and loader
//     chassis.moveToPoint(-0.6_tiles, -0.93_tiles, 1000, false, 100, 6);
//     store();
//     chassis.async().moveToPoint(-1.15_tiles, -0.93_tiles, 800, false, 60, 8);
//     delay(300);
//     loader.extend();
//     chassis.async().moveToPoint(-1.96_tiles, -1.7_tiles, 1000, false, 90, 1);
//     delay(800);
//     score();
//     delay(110);
//     store();

//     // line up loader
//     chassis.turnToHeading(180, 450);
//     chassis.distanceResetPose({&right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      CLOSE LEFT LOADER - 1                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     chassis.moveToPoint(-2_tiles, -55, 1300, false, 90, 6);
//     int loader1Start = millis();
//     chassis.async().moveToPoint(-2_tiles, -60, 1000, false, 40);
//     waitUntilCondition(millis() >= loader1Start + 2000);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           LEFT ALLEY                                           */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive through alley
//     chassis.moveToPoint(-2.55_tiles, -1.1_tiles, 1200, true, 100, 7);
//     chassis.moveToPoint(-2.62_tiles, 0.85_tiles, 1200, true, 100, 7);

//     // swing to scoring
//     chassis.moveToPoint(-2.05_tiles, 1.8_tiles, 1000, true, 100, 2);
//     chassis.turnToHeading(0, 550);
//     chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                    FAR LEFT ALLEY SCORE - 2                                    */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(-2_tiles, 24, 1000, true, 90);
//     delay(800);

//     // start scoring first 6
//     int score2AStart = millis();
//     score();
//     loader.extend();
//     chassis.waitUntilDone();

//     // realign in long goal
//     chassis.turnToHeading(0, 200);
//     chassis.async().driveForDistance(-8, 1000, 70);

//     // wait until scoring timeout
//     waitUntilCondition(millis() >= score2AStart + 1400);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(-2_tiles, 28);
//     // chassis.distanceResetPose({&left_beam, &front_beam}, 3);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                       FAR LEFT LOADER - 2                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     store();
//     chassis.moveToPoint(-2_tiles, 50, 1300, false, 90, 7);
//     int loader2Start = millis();
//     chassis.async().moveToPoint(-2_tiles, 60, 1000, false, 40);
//     waitUntilCondition(millis() >= loader2Start + 1850);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                    FAR LEFT LOADER SCORE - 2                                   */
//     /* ---------------------------------------------------------------------------------------------- */

//     // create a task to timeout when parking
//     bool score2Finished = false;
//     pros::Task score_2 ([&] {
//         // back up to goal
//         chassis.async().moveToPoint(-2_tiles, 24, 1400, true, 90);
//         delay(1000);

//         // start scoring second 6
//         int score2LStart = millis();
//         score();
//         chassis.waitUntilDone();

//         // realign in long goal
//         chassis.turnToHeading(0, 200);
//         chassis.async().driveForDistance(-8, 1000, 70);

//         // wait until scoring done
//         waitUntilCondition(millis() >= score2LStart + 1200);
//         chassis.setPose(-2_tiles, 28);
//         score2Finished = true;
//     });

//     // wait until match timer to guarantee park
//     waitUntilCondition(score2Finished || millis() > startTime + 28900);
//     if (!score2Finished) score_2.remove();
//     delay(15);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                    SETUP FOR BLUE PARK ZONE                                    */
//     /* ---------------------------------------------------------------------------------------------- */

//     // turn out of goals
//     loader.retract();
//     chassis.swingToHeading(90, danielib::SwingSide::LEFT, 500);
//     // mid_ramp.retract();
//     score_mid();

//     // move in front of park zone
//     chassis.moveToPoint(-2, 1.72_tiles, 1400, false, 100);
//     chassis.turnToHeading(0, 500);
//     stop();

//     // distance reset
//     chassis.setPose(0, 40);
//     chassis.distanceResetPose({&front_beam, &left_beam, &right_beam});

//     // back up to setup for zone
//     chassis.moveToPoint(0, 49.2, 880, false, 70, 4);
//     odom_lift.extend();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      CLEAR BLUE PARK ZONE                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive into zone
//     store();
//     left_mg.set_brake_mode_all(MotorBrake::coast);
//     right_mg.set_brake_mode_all(MotorBrake::coast);
//     chassis.driveForDistance(15, 1000, 100);

//     int park2Start = millis();
//     chassis.driveForDistance(5, 500, 70);

//     // wiggle side to side
//     chassis.turnToHeading(-8, 100, 65);
//     chassis.turnToHeading(+8, 100, 65);
//     chassis.turnToHeading(0, 100);

//     // drive fully into wall for last 3 blocks
//     waitUntilCondition(millis() >= park2Start + 950);
//     chassis.driveForDistance(9, 700);

//     // wiggle back and forth
//     chassis.async().driveForDistance(-2, 190);

//     // wiggle side to side
//     chassis.turnToHeading(-14, 130, 75);
//     chassis.turnToHeading(+14, 130, 75);
//     chassis.turnToHeading(0, 130);

//     // back and forth again
//     chassis.driveForDistance(5, 200);
//     chassis.driveForDistance(-2, 150);
//     chassis.driveForDistance(4, 200);

//     // wait until done
//     waitUntilCondition(millis() >= park2Start + 2800);

//     // back all the way out of park zone
//     chassis.driveForDistance(-19, 1100);
//     left_mg.set_brake_mode_all(MotorBrake::brake);
//     right_mg.set_brake_mode_all(MotorBrake::brake);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                   REALIGN AND DISTANCE RESET                                   */
//     /* ---------------------------------------------------------------------------------------------- */

//     // turn around and back up to reset
//     chassis.distanceResetPose({&front_beam});
//     chassis.turnToHeading(180, 600);
//     odom_lift.retract();
//     chassis.moveToPoint(chassis.getPose().x, 49.2, 700, true, 45);
//     stop();

//     // distance reset
//     chassis.setPose(0, 47.6, 180);
//     chassis.distanceResetPose({&left_beam, &right_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                         FAR RIGHT STACK                                        */
//     /* ---------------------------------------------------------------------------------------------- */

//     // get single block from stack
//     store();
//     chassis.async().moveToPoint(16.5, 31, 1600, false, 65);
//     // delay(150);
//     outtake();
//     delay(120);
//     store();

//     // swing to avoid blocks
//     chassis.swingToHeading(80, danielib::SwingSide::LEFT, 350);
//     chassis.turnToHeading(0, 500);
//     chassis.moveToPoint(11, 15.5, 1000, true, 75, 1);
//     chassis.async().swingToHeading(47, danielib::SwingSide::RIGHT, 500);

//     // outtake to unjam
//     bottom.move(-127);
//     top.move(-30);
//     delay(130);
//     bottom.brake();
//     delay(300);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                            MID GOAL                                            */
//     /* ---------------------------------------------------------------------------------------------- */

//     // move back to goal
//     chassis.async().moveToPoint(8, 8, 600, true, 70);

//     // first fast score
//     bottom.move(70);
//     top.move(-55);
//     delay(600);

//     // slow score
//     bottom.move(45);
//     top.move(-30);
//     delay(900);
//     chassis.driveForDistance(1.9, 200);

//     // slowest score
//     bottom.move(30);
//     top.move(-23);
//     delay(950);

//     // back up slow to push
//     chassis.driveForDistance(-4, 200, 30);

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                       FAR RIGHT STACK - 3                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // grab leftover 3 stack
//     store();
//     chassis.async().moveToPoint(1.86_tiles, 1.75_tiles, 1100, false, 100);
//     delay(350);
//     loader.extend();

//     // line up loader
//     chassis.turnToHeading(0, 400);
//     chassis.distanceResetPose({&right_beam});


//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                      FAR RIGHT LOADER - 3                                      */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     chassis.moveToPoint(2_tiles, 55, 1300, false, 90, 6);
//     store();
//     int loader3Start = millis();
//     chassis.async().moveToPoint(2_tiles, 60, 1000, false, 40);
//     waitUntilCondition(millis() >= loader3Start + 2000);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                           RIGHT ALLEY                                          */
//     /* ---------------------------------------------------------------------------------------------- */

//     // drive through alley
//     chassis.moveToPoint(2.55_tiles, 1.1_tiles, 1200, true, 100, 7);
//     chassis.moveToPoint(2.6_tiles, -0.85_tiles, 1200, true, 100, 7);

//     // swing to scoring
//     chassis.moveToPoint(2.09_tiles, -1.8_tiles, 1000, true, 100, 2);
//     chassis.turnToHeading(180, 550);
//     chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                   CLOSE RIGHT ALLEY SCORE - 4                                  */
//     /* ---------------------------------------------------------------------------------------------- */

//     // back up to goal
//     chassis.async().moveToPoint(2.02_tiles, -24, 1000, true, 90);
//     delay(800);

//     // start scoring first 6
//     int score4AStart = millis();
//     score();
//     loader.extend();
//     chassis.waitUntilDone();

//     // realign in long goal
//     chassis.turnToHeading(180, 200);
//     chassis.async().driveForDistance(-8, 1000, 70);

//     // wait until scoring timeout
//     waitUntilCondition(millis() >= score4AStart + 1400);
//     chassis.stopMovement();

//     // distance reset
//     chassis.setPose(2_tiles, -28);
//     chassis.distanceResetPose({&left_beam});

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                     CLOSE RIGHT LOADER - 4                                     */
//     /* ---------------------------------------------------------------------------------------------- */

//     // clear loader
//     store();
//     chassis.moveToPoint(2_tiles, -50, 1300, false, 90, 7);
//     int loader4Start = millis();
//     chassis.async().moveToPoint(2_tiles, -60, 1000, false, 40);
//     score_mid();
//     delay(100);
//     stop();
//     top.move(127);
//     bottom.move(-127);
//     delay(200);
//     bottom.brake();
//     delay(200);
//     store();
//     waitUntilCondition(millis() >= loader4Start + 1850);
//     chassis.stopMovement();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                  CLOSE RIGHT LOADER SCORE - 4                                  */
//     /* ---------------------------------------------------------------------------------------------- */

//     // create a task to timeout when parking
//     bool scoreFinished = false;
//     pros::Task score_4 ([&] {
//         // back up to goal
//         chassis.async().moveToPoint(2_tiles, -24, 1400, true, 90);
//         delay(950);

//         // start scoring second 6
//         int score4LStart = millis();
//         score();
//         loader.retract();
//         chassis.waitUntilDone();

//         // realign in long goal
//         chassis.turnToHeading(180, 200);
//         chassis.async().driveForDistance(-8, 1000, 70);

//         // wait until scoring done
//         waitUntilCondition(millis() >= score4LStart + 1200);
//         chassis.setPose(2_tiles, -28);
//         scoreFinished = true;
//     });

//     // wait until match timer to guarantee park
//     waitUntilCondition(scoreFinished || millis() > startTime + 57800);
//     if (!scoreFinished) score_4.remove();
//     delay(15);
//     chassis.stopMovement();
//     loader.retract();

//     /* ---------------------------------------------------------------------------------------------- */
//     /*                                              PARK                                              */
//     /* ---------------------------------------------------------------------------------------------- */

//     // curve motion to side of blue park zone
//     chassis.moveToPoint(1.7_tiles, -2_tiles, 1500, false, 100, 8);
//     stop();
//     chassis.moveToPoint(24, -62.5, 1500, false, 100, 8);
//     chassis.moveToPoint(13, -63, 1800, false, 100, 8);

//     // drive into park zone with no slew
//     odom_lift.extend();
//     score();
//     chassis.driveForDistance(20, 2000, 100, 0, false);
// }