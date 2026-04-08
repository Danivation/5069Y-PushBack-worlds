#include "auton.hpp"
#include "lemlib-helpers.hpp"
#include "main.h"
using namespace pros;

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

// pros::Color WrongColor = Color::black;
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
void waitUntilColor(pros::Optical* sensor, pros::Color color, int stopTime) {
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
}
void delayMid() {
    pros::delay(60);
}
void delayLong() {
    pros::delay(80);
}
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

/* ---------------------------------------------------------------------------------------------- */
/*                                        NONE (TEST AUTO)                                        */
/* ---------------------------------------------------------------------------------------------- */

void auton_none() {
    
}

/* ---------------------------------------------------------------------------------------------- */
/*                                              SAWPS                                             */
/* ---------------------------------------------------------------------------------------------- */

void auton_sawp_counter_nowing() {

}

void auton_sawp_counter_wing() {
    int startTime = millis();
    c_danielib.setPose(1.75, -47, 270);
    c_lemlib.setPose(1.75, -47, 270);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PUSH AND STEAL PRELOAD                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // steal preload
    store();
    c_danielib.driveForDistance(9, 400);

    // drive backwards towards match loader
    c_lemlib.moveToPoint(1.98_tiles, -1.95_tiles, 1300, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();
    // c_danielib.turnToHeading(180, 500);

    // distance reset
    // lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          RIGHT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.98_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.98_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         RIGHT LONG GOAL                                        */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 700);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(2_tiles, -28, c_lemlib.getPose().theta);
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           MID STACKS                                           */
    /* ---------------------------------------------------------------------------------------------- */
    
    // swing out of long goal
    loader.retract();
    c_danielib.turnToHeading(270, 500);
    store();

    // grab both stacks
    c_lemlib.moveToPoint(0.7_tiles, -0.89_tiles, 1000, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-0.78_tiles, -0.94_tiles, 1300, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    // setup for left side
    c_lemlib.moveToPoint(-1.85_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         LEFT LONG GOAL                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
    delay(560);
    score();
    int score2Start = millis();
    waitUntilCondition(millis() >= score2Start + 800);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(-2_tiles, -28, c_lemlib.getPose().theta);
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    store();
    c_lemlib.moveToPoint(-1.97_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader2Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(-2_tiles, -1.9_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -14, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -10.5, 1200, {.forwards = false, .maxSpeed = 80}, true);
    delay(300);
    top.move(-80);
    bottom.move(70);
    delay(1000);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    c_lemlib.moveToPoint(-2_tiles+0.6_tiles, -1.75_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, false);

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.52_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

void auton_sawp_standard() {

}

void auton_sawp_low_mid() {

}

/* ---------------------------------------------------------------------------------------------- */
/*                                             SPLITS                                             */
/* ---------------------------------------------------------------------------------------------- */

void auton_left_split() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(300);
    loader.extend();

    // js get in the goal somehow ig
    // c_danielib.turnToHeading(80, 500);
    c_lemlib.moveToPoint(-1.6_tiles, -1.07_tiles, 900, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2}, false);
    c_lemlib.swingToHeading(180, lemlib::DriveSide::LEFT, 700, {.maxSpeed = 100}, false);
    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -23, 1000, {.forwards = false}, true);
    // delay(600);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 700);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(-2_tiles, -28, c_lemlib.getPose().theta);
    lemlibDistReset({&right_beam});
    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    store();
    c_lemlib.moveToPoint(-1.97_tiles, -54, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader2Start + 800);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(-2_tiles, -1.8_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -12, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -8, 1200, {.forwards = false, .maxSpeed = 80}, true);
    delay(350);

    // score mid goal kinda slow
    int midScoreStart = millis();
    top.move(-70);
    bottom.move(70);

    // color sensor timeout
    waitUntilColor(&optical_top, Color::blue, midScoreStart + 1500);
    delay(100); // mid color timer
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    c_lemlib.moveToPoint(-2_tiles+0.6_tiles, -1.75_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, false);

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.52_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

void auton_right_split() {
    int startTime = millis();
    c_danielib.setPose(18.5, -49.2, -90);
    c_lemlib.setPose(18.5, -49.2, -90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(1.98_tiles, -2_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(200);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.98_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.98_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 700);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(2_tiles, -28, c_lemlib.getPose().theta);
    // lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      MID STACK + LOW GOAL                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out of long goal
    loader.retract();
    c_danielib.turnToHeading(270, 500);
    store();

    // grab both stacks
    c_lemlib.moveToPoint(0.9_tiles, -0.85_tiles, 1000, {.maxSpeed = 100}, false);
    c_danielib.turnToHeading(-40, 400);
    c_lemlib.moveToPoint(10, -9, 1200, {.maxSpeed = 60}, true);
    delay(500);
    intake_raise.extend();
    c_lemlib.waitUntilDone();

    // intake raise and score
    outtake();
    delay(1000);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(1.55_tiles, -1.7_tiles, 1300, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    stop();
    intake_raise.retract();
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             4 BALLS                                            */
/* ---------------------------------------------------------------------------------------------- */

void auton_left_4ball_loader() {

}

void auton_right_4ball_loader() {
    int startTime = millis();
    c_danielib.setPose(18.5, -49.2, -90);
    c_lemlib.setPose(18.5, -49.2, -90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(1.98_tiles, -2_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(200);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.98_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.98_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 600);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(2_tiles, -28, c_lemlib.getPose().theta);
    // lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    c_lemlib.moveToPoint(2_tiles+0.27_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

void auton_left_4ball_stack() {

}

void auton_right_4ball_stack() {
    int startTime = millis();
    c_danielib.setPose(14, -47, 0);
    c_lemlib.setPose(14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(1.08_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(300);
    loader.extend();
    c_lemlib.moveToPoint(1.35_tiles, -1.35_tiles, 900, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_danielib.swingToHeading(250, danielib::SwingSide::LEFT, 400);
    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(1.95_tiles, -22, 900, {.forwards = false}, true);
    delay(600);
    score();
    int score1Start = millis();
    // delay(200);
    // c_lemlib.cancelMotion();
    // c_lemlib.turnToHeading(180, 400, {}, true);
    waitUntilCondition(millis() >= score1Start + 600);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(2_tiles, -28, c_lemlib.getPose().theta);
    // lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    c_lemlib.moveToPoint(2_tiles+0.27_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             7 BALLS                                            */
/* ---------------------------------------------------------------------------------------------- */

void auton_left_7ball() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(300);
    loader.extend();
    wing.extend();

    // turn around and move towards loader
    c_danielib.turnToHeading(-180, 700);
    c_lemlib.moveToPoint(-1.35_tiles, -1.35_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-1.84_tiles, -2_tiles, 1300, {.minSpeed = 10, .earlyExitRange = 6}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, -57, 800, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    // delay(200);
    outtake();
    delay(100);
    store();
    c_lemlib.waitUntilDone();
    int loader1Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 50}, true);
    waitUntilCondition(millis() >= loader1Start + 1000);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
    delay(850);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 1000);
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.27_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
}

void auton_right_7ball() {

}

/* ---------------------------------------------------------------------------------------------- */
/*                                             SKILLS                                             */
/* ---------------------------------------------------------------------------------------------- */

void auton_skills() {
    int startTime = millis();
    // c_danielib.setPose(0, -50, 180);
    // c_lemlib.setPose(0, -50, 180);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                   REALIGN AND DISTANCE RESET                                   */
    /* ---------------------------------------------------------------------------------------------- */

    // distance reset left and right
    c_danielib.setPose(0, -2_tiles, 0);
    c_lemlib.setPose(0, -2_tiles, 0);
    lemlibDistReset({&left_beam, &right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LOW GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // grab one blue block
    store();
    c_lemlib.moveToPoint(16, -29, 1000, {}, true);
    delay(400);
    outtake();
    delay(70);
    store();

    c_lemlib.moveToPoint(15, -18, 1000, {.minSpeed = 10, .earlyExitRange = 4}, false);
    stop();
    intake_raise.extend();
    c_lemlib.moveToPoint(8, -9, 800, {.maxSpeed = 80}, false);

    // outtake
    bottom.move(-50);
    delay(500);
    bottom.move(-35);
    delay(1400);
    c_danielib.driveForDistance(-2.9, 500);
    bottom.move(-20);
    delay(800);

    // back up
    c_lemlib.moveToPoint(1_tiles, -1_tiles, 800, {.forwards = false, .minSpeed = 10, .earlyExitRange = 4}, false);
    stop();
    intake_raise.retract();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     LEFT STACK + SCORE - 1                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // c_lemlib.moveToPoint()
}