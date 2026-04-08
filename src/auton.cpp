#include "auton.hpp"
#include "lemlib-helpers.hpp"
#include "main.h"
using namespace pros;

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

pros::Color WrongColor = Color::blue;
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
    wing.extend();
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
    wing.retract();
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
    waitUntilColor(&optical_top, WrongColor, midScoreStart + 1500);
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
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    store();

    // grab both stacks
    c_lemlib.moveToPoint(0.86_tiles, -0.91_tiles, 1000, {.maxSpeed = 100}, true);
    loader.extend();
    c_lemlib.waitUntilDone();
    c_danielib.turnToHeading(-40, 400);
    loader.retract();
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
    c_lemlib.moveToPoint(2_tiles+0.43_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    c_lemlib.moveToPoint(-1.98_tiles, -56, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(-1.98_tiles, -70, 1000, {.maxSpeed = 45}, true);
    waitUntilCondition(millis() >= loader1Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LONG GOAL                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1100, {.forwards = false}, true);
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
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    /*                                              WING                                              */
    /* ---------------------------------------------------------------------------------------------- */

    // swing out
    loader.retract();
    wing.retract();
    c_lemlib.moveToPoint(2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    c_lemlib.moveToPoint(-2_tiles, -23, 1000, {.forwards = false}, true);
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
    c_lemlib.moveToPoint(-2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 70, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    c_lemlib.moveToPoint(2_tiles, -23, 1000, {.forwards = false}, true);
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
    c_lemlib.moveToPoint(2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    delay(850);
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
    c_lemlib.moveToPoint(-2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    delay(850);
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
    c_lemlib.moveToPoint(2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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

    c_lemlib.moveToPoint(-2_tiles, -1.8_tiles, 1000, {.forwards = false, .minSpeed = 30, .earlyExitRange = 6}, false);
    c_lemlib.moveToPoint(-12, -12, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-8, -8, 1200, {.forwards = false, .maxSpeed = 80}, true);
    outtake();
    delay(100);
    stop();
    delay(200);

    // score mid goal kinda slow
    int midScoreStart = millis();
    score_mid();

    // color sensor timeout
    waitUntilColor(&optical_top, WrongColor, midScoreStart + 760);
    // delay(100); // mid color timer
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            WING OUT                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // move to wing position
    stop();
    loader.retract();
    wing.extend();
    top.move(127);
    bottom.move(-127);
    delay(100);
    top.brake();
    delay(200);
    stop();
    
    
    // get into position to sweep out
    c_lemlib.moveToPoint(-2_tiles+0.56_tiles, -1.75_tiles, 1100, {.minSpeed = 10, .earlyExitRange = 2}, false);
    c_lemlib.moveToPoint(-2_tiles+0.48_tiles, -1.05_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .minSpeed = 10, .earlyExitRange = 1}, false);

    // sweep out
    wing.retract();
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -1.1_tiles, 1500, {.minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.swingToHeading(150, DriveSide::LEFT, 400, {}, false);
    c_lemlib.moveToPoint(-1.9_tiles, -1.6_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 2}, false);

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
    bool colorStopped = waitUntilColor(&optical_top, WrongColor, score1Start + 800);
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
    c_lemlib.moveToPoint(-2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(-2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    bottom.move(-66);
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
    c_lemlib.moveToPoint(2_tiles+0.35_tiles, -1.7_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 3.5}, false);
    hood.retract();
    stop();

    // back up into wing
    left_mg.set_brake_mode_all(MotorBrake::hold);
    left_mg.set_brake_mode_all(MotorBrake::hold);
    c_lemlib.moveToPoint(2_tiles+0.5_tiles, -1_tiles, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(2_tiles+0.45_tiles, -10, 1500, {.forwards = false, .maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 1}, false);
    c_danielib.turnToHeading(190, 600);
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
    wing.extend();
    c_danielib.setPose(0, -2_tiles, 0);
    c_lemlib.setPose(0, -2_tiles, 0);
    lemlibDistReset({&left_beam, &right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            LOW GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    // grab one blue block
    store();
    c_lemlib.moveToPoint(16, -31, 1000, {}, true);
    delay(300);
    outtake();
    delay(100);
    store();
    c_lemlib.waitUntilDone();
    c_lemlib.swingToHeading(-10, DriveSide::RIGHT, 550, {}, false);

    // line up with the low goal to score
    c_lemlib.moveToPoint(12.5, -18, 1000, {.minSpeed = 10, .earlyExitRange = 1}, false);
    intake_raise.extend();
    c_lemlib.swingToHeading(-45, DriveSide::LEFT, 300, {}, false);
    stop();
    c_lemlib.moveToPoint(8, -9, 800, {.maxSpeed = 50}, false);

    // outtake
    bottom.move(-60);
    top.move(-20);
    delay(350);
    c_danielib.async().driveForDistance(-3, 500);
    bottom.move(-45);
    delay(300);
    bottom.move(-35);
    top.brake();
    delay(1000);
    bottom.move(-30);
    delay(600);

    // back up
    c_lemlib.moveToPoint(1_tiles, -1_tiles, 800, {.forwards = false, .minSpeed = 10, .earlyExitRange = 4}, false);
    stop();
    intake_raise.retract();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT STACK - 1                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // intake 4 left stack
    store();
    c_lemlib.moveToPoint(-0.82_tiles, -1_tiles, 1000, {.minSpeed = 10, .earlyExitRange = 6}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();

    // setup for left side
    c_lemlib.moveToPoint(-1.9_tiles, -1.7_tiles, 1000, {}, false);
    c_lemlib.turnToHeading(180, 300, {}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT SCORE - 1                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 1500, {.forwards = false}, true);
    delay(650);
    score();
    int score1Start = millis();
    waitUntilCondition(millis() >= score1Start + 1000);
    c_lemlib.cancelMotion();
    // c_lemlib.setPose(-2_tiles, -28, c_lemlib.getPose().theta);
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT LOADER - 1                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, -55, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader1Start = millis();
    store();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 38}, true);
    waitUntilCondition(millis() >= loader1Start + 500);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(180+5, 100);
    c_lemlib.turnToHeading(180-5, 100);
    c_lemlib.turnToHeading(180+5, 100);
    c_lemlib.turnToHeading(180-5, 100);
    c_lemlib.turnToHeading(180, 100, {}, false);
    c_danielib.driveForDistance(2, 200);
    c_lemlib.turnToHeading(180+5, 100);
    c_lemlib.turnToHeading(180-5, 100);
    c_lemlib.turnToHeading(180+5, 100);
    c_lemlib.turnToHeading(180-5, 100);
    c_lemlib.turnToHeading(180, 100, {}, false);
    c_danielib.driveForDistance(2, 200);
    waitUntilCondition(millis() >= loader1Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT ALLEY                                           */
    /* ---------------------------------------------------------------------------------------------- */

    // back out of match loader
    c_lemlib.moveToPoint(-2_tiles, -2.1_tiles, 800, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, false);
    loader.retract();

    // backwards through alley
    stop();
    c_lemlib.moveToPoint(-2.6_tiles, -0.95_tiles, 1300, {.forwards = false, .minSpeed = 10, .earlyExitRange = 5}, false);
    c_lemlib.moveToPoint(-2.6_tiles, 0.85_tiles, 1500, {.forwards = false}, false);

    // swing around into goal
    c_lemlib.swingToHeading(-90, DriveSide::LEFT, 600, {.direction = AngularDirection::CW_CLOCKWISE}, false);
    c_lemlib.swingToHeading(0, DriveSide::LEFT, 600, {.direction = AngularDirection::CW_CLOCKWISE}, false);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                       FAR LEFT SCORE - 2                                       */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, 25, 1500, {.forwards = false}, true);
    loader.extend();
    // delay(650);
    delay(100);
    score();
    int score2Start = millis();
    waitUntilCondition(millis() >= score2Start + 1800);
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      CLOSE LEFT LOADER - 1                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    store();
    c_lemlib.moveToPoint(-1.97_tiles, 55, 1300, {.maxSpeed = 80, .minSpeed = 10, .earlyExitRange = 7}, false);
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, 70, 1000, {.maxSpeed = 38}, true);
    waitUntilCondition(millis() >= loader2Start + 500);
    c_lemlib.cancelMotion();
    c_lemlib.turnToHeading(0+5, 100);
    c_lemlib.turnToHeading(0-5, 100);
    c_lemlib.turnToHeading(0+5, 100);
    c_lemlib.turnToHeading(0-5, 100);
    c_lemlib.turnToHeading(0, 100, {}, false);
    c_danielib.driveForDistance(2, 200);
    c_lemlib.turnToHeading(0+5, 100);
    c_lemlib.turnToHeading(0-5, 100);
    c_lemlib.turnToHeading(0+5, 100);
    c_lemlib.turnToHeading(0-5, 100);
    c_lemlib.turnToHeading(0, 100, {}, false);
    c_danielib.driveForDistance(2, 200);
    waitUntilCondition(millis() >= loader2Start + 2000);
    c_lemlib.cancelAllMotions();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                      FAR LEFT SCORE 2 - 2                                      */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, 25, 1500, {.forwards = false}, true);
    delay(900);
    score();
    int score2LStart = millis();
    waitUntilCondition(millis() >= score2LStart + 1800);
    // bottom.move(80);
    // top.move(75);
    // delay(1000);
    c_lemlib.cancelMotion();
    lemlibDistReset({&left_beam});

    stop();
}