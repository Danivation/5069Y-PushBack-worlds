#include "auton.hpp"
#include "lemlib-helpers.hpp"
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
    c_lemlib.moveToPoint(2_tiles, -1.95_tiles, 1300, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          RIGHT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.99_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.99_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 1300);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         RIGHT LONG GOAL                                        */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 850);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           MID STACKS                                           */
    /* ---------------------------------------------------------------------------------------------- */
    
    // swing out of long goal
    loader.retract();
    wing.extend();
    c_danielib.turnToHeading(270, 500);
    score();

    // grab both stacks
    c_lemlib.moveToPoint(0.7_tiles, -0.89_tiles, 1000, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, false);
    store();
    c_lemlib.moveToPoint(-0.78_tiles, -0.94_tiles, 1300, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    // setup for left side
    c_lemlib.moveToPoint(-1.8_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 1.2}, false);

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
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    stop();
    top.move(-127);
    bottom.move(-127);
    c_lemlib.moveToPoint(-1.96_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    delay(250);
    store();
    c_lemlib.waitUntilDone();
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.96_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader2Start + 1300);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to mid goal
    c_lemlib.moveToPoint(-2_tiles, -1.9_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -14, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -10.5, 700, {.forwards = false, .maxSpeed = 80}, true);
    
    delay(300);
    top.move(-96);
    bottom.move(85);
    
    // color sensor timeout
    waitUntilColor(&optical_top, WrongColor, startTime + 16000);
    delay(70); // mid color timer
    stop();
    c_lemlib.cancelMotion();

    // macro and descore
    loader.retract();
    delay(500);
    bottom.move(-127);
    delay(150);
    top.move(127);
    bottom.brake();
    delay(200);
    top.brake();
    bottom.brake();
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
    c_lemlib.moveToPoint(2_tiles, -1.95_tiles, 1300, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          RIGHT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.99_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.99_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 720);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         RIGHT LONG GOAL                                        */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 700);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           MID STACKS                                           */
    /* ---------------------------------------------------------------------------------------------- */
    
    // swing out of long goal
    loader.retract();
    wing.extend();
    c_danielib.turnToHeading(270, 500);
    score();

    // grab both stacks
    c_lemlib.moveToPoint(0.7_tiles, -0.89_tiles, 1000, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, false);
    store();
    c_lemlib.moveToPoint(-0.78_tiles, -0.94_tiles, 1300, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    // setup for left side
    c_lemlib.moveToPoint(-1.8_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 1.2}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         LEFT LONG GOAL                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
    delay(560);
    score();
    int score2Start = millis();
    waitUntilCondition(millis() >= score2Start + 500);
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    top.move(-127);
    bottom.move(-127);
    c_lemlib.moveToPoint(-1.97_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    delay(200);
    store();
    c_lemlib.waitUntilDone();
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader2Start + 720);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to mid goal
    c_lemlib.moveToPoint(-2_tiles, -1.9_tiles, 1000, {.forwards = false, .minSpeed = 80, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -14, 1700, {.forwards = false, .minSpeed = 50, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -10.5, 700, {.forwards = false, .maxSpeed = 80}, true);
    
    delay(60);
    outtake();
    delay(100);
    top.move(-80);
    bottom.move(80);
    
    // color sensor timeout
    waitUntilColor(&optical_top, WrongColor, startTime + 12100);
    delay(100); // mid color timer
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1.67_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, true);
    bottom.move(-127);
    delay(150);
    top.move(127);
    bottom.brake();
    delay(200);
    top.brake();
    bottom.brake();
    c_lemlib.waitUntilDone();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -12, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 3}, false);
    c_danielib.turnToHeading(190, 600);
}

void auton_sawp_low_mid() {

}

/* ---------------------------------------------------------------------------------------------- */
/*                                             SPLITS                                             */
/* ---------------------------------------------------------------------------------------------- */

// left side tuned
void auton_left_split() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
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
    c_lemlib.moveToPoint(-1.97_tiles, -54, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 7}, false);
    store();
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader2Start + 1300);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to mid goal
    c_lemlib.moveToPoint(-2_tiles, -1.8_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -12, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -8, 700, {.forwards = false, .maxSpeed = 80}, true);

    // wait until ready to score
    delay(160);
    bottom.move(-127);
    delay(150);
    stop();
    // waitUntilCondition(millis() >= startTime + 10800);

    // score mid goal kinda slow
    int midScoreStart = millis();
    top.move(-96);
    bottom.move(85);

    // color sensor timeout
    waitUntilColor(&optical_top, WrongColor, midScoreStart + 1300);
    delay(70); // mid color timer
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1.67_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, true);
    bottom.move(-127);
    delay(150);
    top.move(127);
    bottom.brake();
    delay(200);
    top.brake();
    bottom.brake();
    c_lemlib.waitUntilDone();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// right side tuned
void auton_right_split() {
    int startTime = millis();
    c_danielib.setPose(18.5, -49.2, -90);
    c_lemlib.setPose(18.5, -49.2, -90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(1.98_tiles, -1.95_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
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
    // waitUntilCondition(millis() >= score1Start + 700);
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 700);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      MID STACK + LOW GOAL                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out of long goal
    loader.retract();
    c_danielib.turnToHeading(270, 500);
    score();

    // grab both stacks
    c_lemlib.moveToPoint(0.86_tiles, -0.91_tiles, 1000, {.maxSpeed = 100}, true);
    loader.extend();
    c_lemlib.waitUntilDone();
    store();
    c_danielib.turnToHeading(-40, 400);
    loader.retract();
    c_lemlib.moveToPoint(10, -9, 1200, {.maxSpeed = 60}, true);
    delay(500);
    intake_raise.extend();
    c_lemlib.waitUntilDone();

    // intake raise and score
    bottom.move(-60);
    top.move(-40);
    delay(1400);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // front wing
    c_lemlib.moveToPoint(2_tiles-0.56_tiles, -1.4_tiles, 1100, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, false);
    stop();
    intake_raise.retract();
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles-0.45_tiles, -1.05_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles-0.4_tiles, -7, 1000, {}, false);
    c_lemlib.turnToHeading(-15, 5000, {}, false);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             4 BALLS                                            */
/* ---------------------------------------------------------------------------------------------- */

// left side tuned
void auton_left_4ball_loader() {
    int startTime = millis();
    c_danielib.setPose(-18.5, -49.2, 90);
    c_lemlib.setPose(-18.5, -49.2, 90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(-1.98_tiles, -1.95_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(200);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.99_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(-1.99_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 800);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 700);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// copied
void auton_right_4ball_loader() {
    int startTime = millis();
    c_danielib.setPose(18.5, -49.2, -90);
    c_lemlib.setPose(18.5, -49.2, -90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(1.98_tiles, -1.95_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
    delay(200);
    loader.extend();
    c_lemlib.waitUntilDone();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.99_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.99_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 800);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 700);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// left side tuned
void auton_left_4ball_stack() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
    loader.extend();

    // js get in the goal somehow ig
    c_lemlib.moveToPoint(-1.6_tiles, -1.07_tiles, 900, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2}, false);
    c_lemlib.swingToHeading(180, lemlib::DriveSide::LEFT, 700, {.maxSpeed = 100}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1000, {.forwards = false}, true);\
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 700);
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// copied
void auton_right_4ball_stack() {
    int startTime = millis();
    c_danielib.setPose(14, -47, 0);
    c_lemlib.setPose(14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    store();
    c_lemlib.moveToPoint(1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
    loader.extend();

    // js get in the goal somehow ig
    c_lemlib.moveToPoint(1.6_tiles, -1.07_tiles, 900, {.forwards = false, .minSpeed = 25, .earlyExitRange = 2}, false);
    c_lemlib.swingToHeading(180, lemlib::DriveSide::RIGHT, 700, {.maxSpeed = 100}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, -25, 1000, {.forwards = false}, true);\
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 700);
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             7 BALLS                                            */
/* ---------------------------------------------------------------------------------------------- */

// left side tuned
void auton_left_7ball() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // intake 3 stack
    store();
    wing.extend();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
    loader.extend();

    // setup movement
    c_lemlib.turnToHeading(-150, 550, {}, false);
    c_lemlib.moveToPoint(-1.86_tiles, -1.7_tiles, 1300, {.minSpeed = 50, .earlyExitRange = 5}, false);
    c_lemlib.turnToHeading(180, 200, {.earlyExitRange = 5}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, -57, 800, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    outtake();
    delay(150);
    store();
    c_lemlib.waitUntilDone();
    int loader1Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1500, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 1000);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 1300);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// copied
void auton_right_7ball() {
    int startTime = millis();
    c_danielib.setPose(14, -47, 0);
    c_lemlib.setPose(14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // intake 3 stack
    store();
    wing.extend();
    c_lemlib.moveToPoint(1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
    loader.extend();

    // setup movement
    c_lemlib.turnToHeading(150, 550, {}, false);
    c_lemlib.moveToPoint(1.86_tiles, -1.7_tiles, 1300, {.minSpeed = 50, .earlyExitRange = 5}, false);
    c_lemlib.turnToHeading(180, 200, {.earlyExitRange = 5}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.97_tiles, -57, 800, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    outtake();
    delay(150);
    store();
    c_lemlib.waitUntilDone();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.97_tiles, -70, 1500, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 1000);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    wing.retract();
    c_lemlib.moveToPoint(2_tiles, -25, 1100, {.forwards = false}, true);
    delay(800);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 1300);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                           OTHER AUTOS                                          */
/* ---------------------------------------------------------------------------------------------- */

// left side tuned
void auton_left_7ball_counter() {
    int startTime = millis();
    c_danielib.setPose(-14, -47, 0);
    c_lemlib.setPose(-14, -47, 0);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              STACK                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // intake 3 stack
    store();
    wing.extend();
    c_lemlib.moveToPoint(-1.05_tiles, -0.88_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, true);
    delay(240);
    loader.extend();

    // setup movement
    c_lemlib.turnToHeading(-150, 550, {}, false);
    c_lemlib.moveToPoint(-1.86_tiles, -1.7_tiles, 1300, {.minSpeed = 50, .earlyExitRange = 5}, false);
    c_lemlib.turnToHeading(180, 200, {.minSpeed = 10, .earlyExitRange = 5}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             LOADER                                             */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, -57, 800, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 6.5}, true);
    outtake();
    delay(150);
    store();
    c_lemlib.waitUntilDone();
    int loader1Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1500, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 860);
    c_lemlib.cancelMotion();

    
    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to mid goal
    c_lemlib.moveToPoint(-2_tiles, -1.8_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -12, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -8, 1200, {.forwards = false, .maxSpeed = 80}, true);
    outtake();
    delay(100);
    stop();
    delay(200);

    // wait until ready to score
    waitUntilCondition(millis() >= startTime + 6200);

    // score mid goal kinda slow
    int midScoreStart = millis();
    top.move(-96);
    bottom.move(85);

    // color sensor timeout
    waitUntilColor(&optical_top, WrongColor, midScoreStart + 610);
    delay(70); // mid color timer
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            WING OUT                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    wing.extend();
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1.67_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, true);
    bottom.move(-127);
    delay(150);
    top.move(127);
    bottom.brake();
    delay(200);
    top.brake();
    bottom.brake();
    c_lemlib.waitUntilDone();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.47_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.43_tiles, -10, 1500, {.forwards = false, .minSpeed = 10, .earlyExitRange = 1}, false);

    // sweep out
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1.2_tiles, 1500, {.minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.swingToHeading(150, DriveSide::LEFT, 400, {}, false);
    c_lemlib.moveToPoint(-1.9_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         LONG GOAL SCORE                                        */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    store();
    c_lemlib.turnToHeading(180, 400, {.minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles, -23, 1500, {.forwards = false}, true);
    delay(700);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 950);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

// right side tuned
void auton_right_7ball_counter() {
    int startTime = millis();
    c_danielib.setPose(18.5, -49.2, -90);
    c_lemlib.setPose(18.5, -49.2, -90);

    // drive backwards towards match loader
    store();
    c_lemlib.moveToPoint(1.98_tiles, -1.95_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, true);
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
    /*                                      MID STACK + LOW GOAL                                      */
    /* ---------------------------------------------------------------------------------------------- */
    
    // move out of loader
    c_lemlib.moveToPoint(2_tiles, -1.8_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 2}, false);

    // swing out of long goal
    loader.retract();
    c_danielib.turnToHeading(-45, 500);
    store();

    // grab both stacks
    c_lemlib.moveToPoint(0.86_tiles, -0.91_tiles, 1000, {.maxSpeed = 100}, true);
    delay(400);
    loader.extend();
    c_lemlib.waitUntilDone();
    c_danielib.async().turnToHeading(-40, 400);
    outtake();
    delay(120);
    store();
    c_danielib.waitUntilDone();
    loader.retract();
    c_lemlib.moveToPoint(10, -9, 1200, {.maxSpeed = 60}, true);
    delay(300);
    intake_raise.extend();
    c_lemlib.waitUntilDone();

    // intake raise and score
    // outtake();
    bottom.move(-75);
    delay(400);
    stop();
    intake_raise.retract();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         WING SWEEP OUT                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    loader.retract();
    wing.extend();
    
    // get into position to sweep out
    c_lemlib.moveToPoint(2_tiles-0.56_tiles, -1.4_tiles, 1100, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, false);
    store();
    c_lemlib.moveToPoint(2_tiles-0.45_tiles, -1.05_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles-0.4_tiles, -7, 1000, {}, false);
    c_lemlib.turnToHeading(-9, 200, {}, false);

    // sweep out
    wing.retract();
    c_lemlib.moveToPoint(2_tiles-0.42_tiles, -0.9_tiles, 1500, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.swingToHeading(30, DriveSide::LEFT, 300, {}, false);
    c_lemlib.moveToPoint(2_tiles, -1.6_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         LONG GOAL SCORE                                        */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    store();
    c_lemlib.turnToHeading(180, 400, {.minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles, -23, 1500, {.forwards = false}, true);
    delay(700);
    score();
    int score1Start = millis();
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 1000);
    if (colorStopped) {
        delayLong();
        hood.retract();
        stop();
    }
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(2_tiles+0.41_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.42_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(195);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             SKILLS                                             */
/* ---------------------------------------------------------------------------------------------- */

void auton_skills() {
    int startTime = millis();
    c_danielib.setPose(0, -53, 180);
    c_lemlib.setPose(0, -53, 180);
    wing.extend();
    odom_lift.extend();
    store();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          RED PARK ZONE                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // drive in
    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    c_danielib.driveForDistance(3.2, 410, 40, 0, false);

    // first row wiggles
    c_lemlib.turnToHeading(180+5, 150);
    c_lemlib.turnToHeading(180-5, 150);
    c_lemlib.turnToHeading(180+5, 150);
    c_lemlib.turnToHeading(180, 200, {}, false);

    // second row wiggles
    c_danielib.async().driveForDistance(8, 600, 120, 0, false);
    delay(160);
    // intake_raise.extend();
    delay(400);
    intake_raise.retract();
    c_danielib.waitUntilDone();

    // turns
    c_lemlib.turnToHeading(180+7, 150);
    c_lemlib.turnToHeading(180-7, 150);
    c_lemlib.turnToHeading(180, 200, {}, false);

    // NEW BACK AND FORTH TECH
    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    c_danielib.async().driveForDistance(-5, 300, 120, 0, false);
    delay(200);
    bottom.move(-127);
    delay(100);
    store();
    c_danielib.driveForDistance(7, 250, 120, 0, false);
    c_lemlib.turnToHeading(180+5, 100);
    c_lemlib.turnToHeading(180-5, 100, {}, false);
    c_danielib.driveForDistance(-5, 250, 100, 0, true);
    bottom.move(-127);
    delay(200);
    store();
    delay(30);
    c_danielib.driveForDistance(7, 150, 120, 0, false);

    // back out
    int backoutTime = millis();
    c_danielib.async().driveForDistance(-18.5, 1500);
    loader.extend();
    waitUntilCondition((millis() >= backoutTime + 1000) || (distance_front.get_distance() > 600 && distance_front.get_distance() < 2000));
    c_danielib.stopMovement();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                    REALIGN + DISTANCE RESET                                    */
    /* ---------------------------------------------------------------------------------------------- */

    // front reset
    left_mg.set_brake_mode_all(MotorBrake::brake);
    right_mg.set_brake_mode_all(MotorBrake::brake);
    odom_lift.retract();
    lemlibDistReset({&left_beam, &right_beam}, 10, 10);
    lemlibDistReset({&front_beam}, 8, 8);

    // HEADING RESET - NEW
    auto redPose1 = c_lemlib.getPose();
    c_danielib.driveForDistance(8, 500, 30);
    auto redPose = c_lemlib.getPose();
    if (redPose.y > -48.0f && redPose.y < -44.5f && redPose.theta < 195 && redPose.theta > 165) {
        c_lemlib.setPose(redPose.x, redPose.y, 180);
        lemlibDistReset({&left_beam, &right_beam}, 10, 10);
        lemlibDistReset({&front_beam}, 8, 8);
    }

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LOW GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // grab one block
    c_danielib.driveForDistance(-10, 170, 120, 5);
    loader.retract();
    store();
    c_lemlib.turnToHeading(45, 500, {}, false);

    // unjam during movement
    c_lemlib.moveToPoint(15.9, -30, 1300, {}, true);
    delay(200);
    bottom.move(-127);
    delay(150);
    store();
    c_lemlib.waitUntilDone();

    // swing away from stack
    c_danielib.driveForDistance(-7, 320, 100);
    c_danielib.turnToHeading(0, 300);

    // line up with low goal
    c_lemlib.moveToPoint(10, -16, 1000, {.minSpeed = 40, .earlyExitRange = 2}, false);
    intake_raise.extend();
    c_lemlib.swingToHeading(-45, DriveSide::LEFT, 500, {}, false);
    c_lemlib.moveToPose(7.5, -7.5, -45, 700, {.horizontalDrift = 4, .lead = 0.55}, true);
    delay(200);

    // outtake
                bottom.move_velocity(-60);
                top.move(-30);
                delay(300);

                bottom.move_velocity(-55);
                delay(400);

                bottom.move_velocity(-52);
                top.brake();
                delay(500);
                c_danielib.async().driveForDistance(-2.3, 500);
                delay(500);

                bottom.move_velocity(-40);
                delay(400);
                // bottom.move_velocity(-40);
                delay(500);

                // BRAKE AND START DRIVING

                c_danielib.driveForDistance(-4, 500);

    // back up
    c_lemlib.swingToHeading(10, DriveSide::LEFT, 500, {}, false);
    
    c_lemlib.moveToPoint(0.35_tiles, -0.8_tiles, 1000, {.forwards = false, .maxSpeed = 90, .minSpeed = 20, .earlyExitRange = 3}, true);
    delay(200);
    intake_raise.retract();
    bottom.move(-100);
    c_lemlib.waitUntilDone();
    stop();
    c_lemlib.turnToHeading(270, 300, {}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT STACK - 1                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // intake 4 left stack
    store();
    c_lemlib.moveToPoint(-0.82_tiles, -0.93_tiles, 1000, {.minSpeed = 70, .earlyExitRange = 7}, true);
    delay(730);
    loader.extend();
    c_lemlib.waitUntilDone();

    // setup for left side
    c_lemlib.moveToPoint(-1.9_tiles, -1.55_tiles, 850, {.maxSpeed = 90, .minSpeed = 30, .earlyExitRange = 2}, false);
    c_danielib.turnToHeading(180, 400);
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT SCORE - 1                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -24, 1000, {.forwards = false, .maxSpeed = 90}, true);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    delay(350);
    int score1Start = millis();
    score();
    waitUntilCondition(millis() >= score1Start + 600);
    c_lemlib.cancelMotion();
    bottom.move(127);
    top.move(-127);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT LOADER - 1                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.99_tiles, -54, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader1Start = millis();
    store();
    c_lemlib.moveToPoint(-1.99_tiles, -70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader1Start + 750);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(180+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    waitUntilCondition(millis() >= loader1Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT ALLEY                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // back out of match loader
    c_lemlib.moveToPoint(-2_tiles, -2_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 6}, false);
    
    // backwards through alley
    c_lemlib.moveToPoint(-2.58_tiles, -1.1_tiles, 1300, {.forwards = false, .minSpeed = 70, .earlyExitRange = 7}, false);
    loader.retract();
    c_lemlib.moveToPoint(-2.58_tiles, 1_tiles, 1300, {.forwards = false, .minSpeed = 70, .earlyExitRange = 6}, false);
    lemlibDistReset({&right_beam});

    // swing around into goal
    c_lemlib.moveToPoint(-2_tiles, 1.77_tiles, 1100, {.forwards = false, .maxSpeed = 90}, false);
    c_danielib.turnToHeading(0, 600);
    lemlibDistReset({&left_beam, &front_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       FAR LEFT SCORE - 2                                       */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    loader.extend();
    c_lemlib.moveToPoint(-2.05_tiles, 26, 800, {.forwards = false, .maxSpeed = 90}, true);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    delay(350);
    int score2Start = millis();
    score();

    // realign inside goal
    c_lemlib.moveToPoint(-2.05_tiles, 22, 2000, {.forwards = false, .maxSpeed = 70}, true);
    waitUntilCondition(millis() >= score2Start + 1100);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       FAR LEFT LOADER - 2                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, 54, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader2Start = millis();
    store();
    c_lemlib.moveToPoint(-1.97_tiles, 70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader2Start + 750);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(0+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    waitUntilCondition(millis() >= loader2Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      FAR LEFT SCORE 2 - 2                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2.03_tiles, 24, 1300, {.forwards = false, .maxSpeed = 90}, true);
    delay(400);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    loader.retract();
    delay(400);
    int score2LStart = millis();
    bottom.move(70);
    top.move(65);

    // realign inside goal and wait until global timeout
    c_lemlib.moveToPoint(-2.03_tiles, 22, 2000, {.forwards = false, .maxSpeed = 70}, true);
    waitUntilCondition(millis() >= startTime + 27400 || millis() >= score2LStart + 1800);
    top.move(127);
    bottom.move(127);
    delay(100);
    c_lemlib.cancelMotion();

    // dist reset
    // c_lemlib.setPose(-2_tiles, 27, 0);
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         BLUE PARK ZONE                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // drive to park
    loader.retract();
    score();
    c_lemlib.moveToPoint(-1.8_tiles, 1.9_tiles, 1000, {.minSpeed = 75, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-1.23_tiles, 2.5_tiles, 1000, {.minSpeed = 75, .earlyExitRange = 6}, false);
    lemlibDistReset({&left_beam});
    c_lemlib.moveToPoint(-16, 63.5, 1000, {}, false);

    // START drive in
    store();
    odom_lift.extend();
    delay(100);
    lemlibDistReset({&left_beam});
    c_lemlib.moveToPoint(1, 64, 1500, {.maxSpeed = 95, .minSpeed = 65, .earlyExitRange = 4}, false);

    lemlibDistReset({&left_beam});
    
    // drive through and a little bit out
    c_lemlib.moveToPoint(16, 64, 1200, {.maxSpeed = 100, .minSpeed = 55, .earlyExitRange = 4}, true);
    odom_lift.retract();
    loader.extend();    //"sidih is the goat"!!!!!
    c_lemlib.waitUntilDone();

    // drive all the way out
    c_lemlib.moveToPoint(26.5, 64, 1200, {.minSpeed = 40, .earlyExitRange = 3}, true);
    delay(220);

    // dist reset in the middle
    lemlibDistReset({&front_beam, &left_beam});
    loader.retract();
    c_lemlib.waitUntilDone();

    // swing and scoop
    c_lemlib.swingToHeading(135, DriveSide::RIGHT, 400, {}, false);
    c_lemlib.turnToHeading(50, 330, {}, false);
    c_lemlib.swingToHeading(0, DriveSide::LEFT, 500, {}, false);
    c_danielib.driveForDistance(8, 400);
    
    // reset on wall
    lemlibDistReset({&front_beam, &right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          HIGH MID GOAL                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // get block
    store();
    loader.retract();
    c_lemlib.moveToPoint(0.4_tiles, 1.85_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 2}, false);
    c_lemlib.turnToHeading(180-45, 300, {}, false);
    c_lemlib.moveToPoint(15.5, 30, 1000, {}, true);
    delay(200);
    bottom.move(-127);
    delay(100);
    store();
    c_lemlib.waitUntilDone();

    // swing away from stack
    c_danielib.driveForDistance(-6, 500);
    c_danielib.turnToHeading(0, 400);

    // line up with mid goal
    c_lemlib.moveToPoint(10, 17, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 3}, false);
    c_lemlib.moveToPose(6, 7, 45, 850, {.forwards = false, .horizontalDrift = 4, .lead = 0.4}, false);

    // score mid goal
                c_danielib.async().driveForDistance(-4, 500, 10);
                bottom.move(-100);
                top.move(-30);
                delay(130);

                top.move_velocity(-105);
                bottom.move_velocity(90);
                delay(450);

                top.move_velocity(-90);
                bottom.move_velocity(65);
                delay(850);

                top.move_velocity(-85);
                bottom.move_velocity(58);
                delay(1000);

                top.move_velocity(-60);
                bottom.move_velocity(60);
                delay(1100);

                c_danielib.driveForDistance(-5, 350, 15);
                top.brake();
                bottom.brake();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       FAR RIGHT STACK - 3                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // intake leftover 3 stack
    c_danielib.stopMovement();
    c_lemlib.cancelMotion();
    c_lemlib.moveToPoint(1.92_tiles, 1.65_tiles, 1400, {.maxSpeed = 90}, true);
    delay(100);
    score_mid();
    delay(250);
    loader.extend();
    delay(200);
    store();
    c_lemlib.waitUntilDone();

    // setup to score
    c_danielib.turnToHeading(0, 400);
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       FAR RIGHT SCORE - 3                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2_tiles, 24, 1000, {.forwards = false, .maxSpeed = 90}, true);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    delay(300);
    int score3Start = millis();
    score();

    // wait until done
    waitUntilCondition(millis() >= score3Start + 600);
    c_lemlib.cancelMotion();
    bottom.move(127);
    top.move(-127);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      FAR RIGHT LOADER - 3                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.99_tiles, 54, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader3Start = millis();
    store();
    c_lemlib.moveToPoint(1.99_tiles, 70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader3Start + 750);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(0+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(0-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    waitUntilCondition(millis() >= loader3Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           RIGHT ALLEY                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // back out of match loader
    c_lemlib.moveToPoint(2_tiles, 2_tiles, 1000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 6}, false);
    
    // backwards through alley
    c_lemlib.moveToPoint(2.58_tiles, 1.1_tiles, 1300, {.forwards = false, .minSpeed = 70, .earlyExitRange = 7}, false);
    loader.retract();
    c_lemlib.moveToPoint(2.58_tiles, -1_tiles, 1300, {.forwards = false, .minSpeed = 70, .earlyExitRange = 6}, false);
    lemlibDistReset({&right_beam});

    // swing around into goal
    c_lemlib.moveToPoint(2_tiles, -1.77_tiles, 1100, {.forwards = false, .maxSpeed = 90}, false);
    c_danielib.turnToHeading(180, 600);
    lemlibDistReset({&left_beam, &front_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE RIGHT SCORE - 4                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    loader.extend();
    c_lemlib.moveToPoint(2.05_tiles, -26, 800, {.forwards = false, .maxSpeed = 90}, true);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    delay(350);
    int score4Start = millis();
    score();

    // realign inside goal
    c_lemlib.moveToPoint(2.05_tiles, -22, 2000, {.forwards = false, .maxSpeed = 70}, true);
    waitUntilCondition(millis() >= score4Start + 1100);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     CLOSE RIGHT LOADER - 4                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.97_tiles, -54, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader4Start = millis();
    store();
    c_lemlib.moveToPoint(1.97_tiles, -70, 1000, {.maxSpeed = 35}, true);
    waitUntilCondition(millis() >= loader4Start + 750);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(180+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180+5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    c_lemlib.turnToHeading(180-5, 100, {}, false);
    c_danielib.driveForDistance(3, 100);
    waitUntilCondition(millis() >= loader4Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     CLOSE RIGHT SCORE 2 - 4                                    */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(2.05_tiles, -26, 1300, {.forwards = false, .maxSpeed = 90}, true);
    delay(400);
    stop();
    delay(200);
    hood.extend();
    top.move(-15);
    bottom.move(-20);
    delay(400);
    int score4LStart = millis();
    bottom.move(70);
    top.move(65);

    // realign inside goal and wait until global timeout
    c_lemlib.moveToPoint(2.05_tiles, -22, 2000, {.forwards = false, .maxSpeed = 70}, true);
    waitUntilCondition(millis() >= startTime + 56700 || millis() >= score4LStart + 1700);
    top.move(127);
    bottom.move(127);
    delay(200);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              PARK                                              */
    /* ---------------------------------------------------------------------------------------------- */

    loader.retract();
    score();
    c_lemlib.moveToPose(15, -63, -90, 1500, {.horizontalDrift = 3.5, .lead = 0.55, .minSpeed = 30, .earlyExitRange = 8.5}, false);
    lemlibDistReset({&left_beam});
    c_lemlib.moveToPoint(17, -62.5, 1000, {.maxSpeed = 90}, false);

    // drive in
    store();
    odom_lift.extend();
    c_lemlib.moveToPoint(0, -64, 1500, {.maxSpeed = 95}, false);
}
