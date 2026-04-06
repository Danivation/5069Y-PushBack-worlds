#include "lemlib-helpers.hpp"
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

/* ---------------------------------------------------------------------------------------------- */
/*                                        NONE (TEST AUTO)                                        */
/* ---------------------------------------------------------------------------------------------- */

// PUSH COUNTER SAWP
void auton_none() {
    int startTime = millis();
    c_danielib.setPose(2, -47.5, 270);
    c_lemlib.setPose(2, -47.5, 270);

    /* ---------------------------------------------------------------------------------------------- */
    /*                                     PUSH AND STEAL PRELOAD                                     */
    /* ---------------------------------------------------------------------------------------------- */

    // steal preload
    store();
    c_danielib.driveForDistance(9, 400);

    // drive backwards towards match loader
    c_lemlib.moveToPoint(1.98_tiles, -1.9_tiles, 1300, {.forwards = false}, true);
    delay(700);
    loader.extend();
    c_lemlib.waitUntilDone();
    c_danielib.turnToHeading(180, 500);

    // distance reset
    lemlibDistReset({&left_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                          RIGHT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(1.99_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader1Start = millis();
    c_lemlib.moveToPoint(1.99_tiles, -70, 1000, {.maxSpeed = 40}, true);
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

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           MID STACKS                                           */
    /* ---------------------------------------------------------------------------------------------- */
    
    // swing out of long goal
    loader.retract();
    c_danielib.turnToHeading(270, 500);
    store();

    // grab both stacks
    c_lemlib.moveToPoint(0.7_tiles, -0.9_tiles, 1000, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, false);
    c_lemlib.moveToPoint(-0.78_tiles, -0.97_tiles, 1300, {.maxSpeed = 100, .minSpeed = 10, .earlyExitRange = 7}, false);
    loader.extend();

    // setup for left side
    c_lemlib.moveToPoint(-1.93_tiles, -2_tiles, 1100);
    c_danielib.turnToHeading(180, 500);

    // distance reset
    lemlibDistReset({&right_beam});

    /* ---------------------------------------------------------------------------------------------- */
    /*                                         LEFT LONG GOAL                                         */
    /* ---------------------------------------------------------------------------------------------- */

    // drive backwards to goal
    c_lemlib.moveToPoint(-2_tiles, -25, 800, {.forwards = false}, true);
    delay(580);
    score();
    int score2Start = millis();
    waitUntilCondition(millis() >= score2Start + 900);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                           LEFT LOADER                                          */
    /* ---------------------------------------------------------------------------------------------- */

    // drive into loader
    c_lemlib.moveToPoint(-1.97_tiles, -55, 1300, {.maxSpeed = 90, .minSpeed = 10, .earlyExitRange = 6.5}, false);
    store();
    int loader2Start = millis();
    c_lemlib.moveToPoint(-1.97_tiles, -70, 1000, {.maxSpeed = 40}, true);
    waitUntilCondition(millis() >= loader2Start + 750);
    c_lemlib.cancelMotion();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                            MID GOAL                                            */
    /* ---------------------------------------------------------------------------------------------- */

    c_lemlib.moveToPoint(-2_tiles, -2.2_tiles, 800, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7.5}, true);
    c_lemlib.moveToPoint(-13, -13, 1700, {.forwards = false, .minSpeed = 10, .earlyExitRange = 7}, true);
    c_lemlib.moveToPoint(-9, -9, 1200, {.forwards = false, .maxSpeed = 80}, true);
    delay(800);
    top.move(-80);
    bottom.move(70);
    delay(1000);
}
