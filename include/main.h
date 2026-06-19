/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2024, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_
#define PROS_USE_SIMPLE_NAMES

#include "api.h" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "danielib/danielib.hpp" // IWYU pragma: keep

#ifdef __cplusplus
#include <math.h> // IWYU pragma: keep
#include <stdio.h> // IWYU pragma: keep
#include <stdlib.h> // IWYU pragma: keep
#include <string.h> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <cmath> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep
#endif


/**
 * Custom includes go here
 */
#include "auton.hpp" // IWYU pragma: keep
#include "driver.hpp" // IWYU pragma: keep
#include "extras.hpp" // IWYU pragma: keep
#include "lemlib-helpers.hpp" // IWYU pragma: keep

extern const bool skillsSlow;
extern const bool autoForDriver;

/**
 * Robot configuration
 */
extern pros::Controller master;
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;

extern pros::MotorGroup roller;
extern pros::MotorGroup lift;
extern pros::Motor intake;

extern pros::adi::Pneumatics claw;
extern pros::adi::Pneumatics piston2;




extern pros::Rotation horizontal_rotation;
extern pros::Rotation vertical_rotation;
extern CustomImu imu_1;
// extern pros::Imu imu_2;
extern pros::Optical optical_top;
extern pros::Distance distance_left;
extern pros::Distance distance_right;
extern pros::Distance distance_front;

extern pros::adi::Pneumatics loader;
extern pros::adi::Pneumatics wing;
extern pros::adi::Pneumatics hood;
extern pros::adi::Pneumatics mid_descore;
extern pros::adi::Pneumatics odom_lift;
extern pros::adi::Pneumatics intake_raise;

extern lemlib::Chassis c_lemlib;
extern danielib::Drivetrain c_danielib;
extern danielib::Beam left_beam;
extern danielib::Beam right_beam;
extern danielib::Beam front_beam;

#define waitUntilCondition(condition)    \
  do {                          \
    pros::delay(5);             \
  } while (!(condition))

template <typename Func>
void waitUntilFunction(Func condition) {
  while (!condition()) pros::delay(5);
}


/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
using namespace pros;
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif
#endif  // _PROS_MAIN_H_
