#include "main.h"
#include "pros/abstract_motor.hpp"


/* ---------------------------------------------------------------------------------------------- */
/*                                          DEVICE PORTS                                          */
/* ---------------------------------------------------------------------------------------------- */

// MASTER CONTROLLER
pros::Controller master(pros::E_CONTROLLER_MASTER);

// DRIVE GEAR LAYOUT: O-m--M-MO
pros::MotorGroup left_mg({6, -7, -8}, pros::MotorGears::blue);      // 6: 5.5W, 7/8: 11W
pros::MotorGroup right_mg({-3, 4, 5}, pros::MotorGears::blue);    // 1: 3: 5.5W, 4/5: 11W

// MECHANISM MOTORS
pros::MotorGroup lift({-10, 9}, pros::MotorGears::green, pros::MotorEncoderUnits::degrees);
pros::Motor cone(-1, pros::MotorGears::rpm_200);
pros::Motor wrist(2, pros::MotorGears::rpm_200);
pros::Motor intake(19, pros::MotorGears::rpm_200);

// ODOMETRY SENSORS
CustomImu imu_1(18, 360.0f/358.95f);
pros::Rotation vertical_rotation(20);
pros::Rotation horizontal_rotation(17);

// OTHER SENSORS
pros::Rotation lift_rot(12);
pros::Rotation wrist_rot(13);
pros::Distance pin_dist(21);

pros::Distance distance_left(16);
pros::Distance distance_right(14);
pros::Distance distance_back(15);


pros::Distance distance_front(22);
pros::Optical optical_top(22);

// PNEUMATICS
pros::adi::Pneumatics claw('A', false);
pros::adi::Pneumatics odom_lift('H', false);
pros::adi::Pneumatics hood('C', false);
pros::adi::Pneumatics wing('D', false);
pros::adi::Pneumatics intake_raise('E', false);
pros::adi::Pneumatics mid_descore('F', false);
pros::adi::Pneumatics loader('G', false);


/* ---------------------------------------------------------------------------------------------- */
/*                                              PIDS                                              */
/* ---------------------------------------------------------------------------------------------- */

// CASCADE LIFT PID
danielib::PID liftPID(5.2, 0.25, 58, 5, 0, 0, 5);

// WRIST PID
danielib::PID wristPID(1.5, 0.08, 0, 10, 0, 0, 0);
// danielib::PID wristStaticPID(1.4, 0.05, 0, 5, 0, 0, 0);

// LEMLIB LINEAR PID - very good for mtp, no tip
lemlib::ControllerSettings lateral_controller(7.9, 0, 30, 0, 1, 100, 2.5, 300, 9);

// LEMLIB ANGULAR PID - good for mtp, little end turning, but may not be good for normal turns
lemlib::ControllerSettings angular_controller(5.95, 0, 38, 0, 1, 85, 3, 300, 0);

// DANIELIB ANGULAR PID - tuned for normal turns
danielib::PID angularPID(3.76, 0.15, 16.5, 5, 1.5, 130, 0);
danielib::PID swingAngularPID(6.6, 0.2, 32, 4, 2, 180, 0);

// DANIELIB LINEAR PID - tuned for straight drives
danielib::PID linearPID(8.2, 0, 22, 0, 1, 200, 8);

// DANIELIB MTP - somewhat tuned, but not very optimal
danielib::PID mtpLinearPID(7.95, 0, 23.5, 0, 1, 100, 9);
danielib::PID mtpAngularPID(3.3, 0, 15, 0, 0, 0, 0); // tuned?

/* ---------------------------------------------------------------------------------------------- */
/*                                          LEMLIB CONFIG                                         */
/* ---------------------------------------------------------------------------------------------- */

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 12.0, lemlib::Omniwheel::NEW_275, 450, 2);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation, 2, 0);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_rotation, 2, 2.25);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, &horizontal_tracking_wheel, nullptr, &imu_1);

// create the chassis
lemlib::Chassis c_lemlib(drivetrain, lateral_controller, angular_controller, sensors);

/* ---------------------------------------------------------------------------------------------- */
/*                                         DANIELIB CONFIG                                        */
/* ---------------------------------------------------------------------------------------------- */

// STILL USED FOR DISTANCE RESETS
danielib::Beam left_beam(-90, -4.5, -2.25, distance_left);
danielib::Beam right_beam(90, 4.5, -2.25, distance_right);
danielib::Beam front_beam(0, 4.7, 3.7, distance_front);

// + offset is right or front, - offset is left or back
danielib::TrackerWheel vertical_tracker_danielib(vertical_rotation, 2, 0);
danielib::TrackerWheel horizontal_tracker_danielib(horizontal_rotation, 2, 2.25);
danielib::Inertial inertial(imu_1);

danielib::Localization mcl({left_beam, right_beam, front_beam});
danielib::Sensors sensors_danielib(vertical_tracker_danielib, horizontal_tracker_danielib, inertial, mcl);

danielib::Drivetrain c_danielib(left_mg, right_mg, sensors_danielib, 12.0, 2.75, 450, linearPID, angularPID, mtpLinearPID, mtpAngularPID, swingAngularPID);
