#include "main.h"


/* ---------------------------------------------------------------------------------------------- */
/*                                          DEVICE PORTS                                          */
/* ---------------------------------------------------------------------------------------------- */

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({13, -12, -11}, pros::MotorGears::blue);
pros::MotorGroup right_mg({-18, 19, 20}, pros::MotorGears::blue);

pros::MotorGroup bottom({9, 8}, pros::MotorGears::blue);
pros::Motor top(1, pros::MotorGears::rpm_200);

CustomImu imu_1(15, 360.0f/356.6f);
pros::Imu imu_2(22); // unused
pros::Rotation vertical_rotation(-17);
pros::Rotation horizontal_rotation(14);

pros::Optical optical_top(22); // unused
pros::Distance distance_front(3);
pros::Distance distance_left(4);
pros::Distance distance_right(10);

pros::adi::Pneumatics loader('A', false);
pros::adi::Pneumatics wing('B', false);
pros::adi::Pneumatics hood('C', false);
pros::adi::Pneumatics mid_descore('D', false);
pros::adi::Pneumatics intake_raise('E', false);
pros::adi::Pneumatics odom_lift('F', false);

/* ---------------------------------------------------------------------------------------------- */
/*                                          LEMLIB CONFIG                                         */
/* ---------------------------------------------------------------------------------------------- */

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 11.3, lemlib::Omniwheel::NEW_325, 450, 2);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation, lemlib::Omniwheel::NEW_2, -0.5);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_rotation, lemlib::Omniwheel::NEW_2, -2.4);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, &horizontal_tracking_wheel, nullptr, &imu_1);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2.5, // proportional gain (kP)
                                              0.0, // integral gain (kI)
                                              16.1, // derivative gain (kD)
                                              5, // anti windup
                                              1, // small error range, in degrees
                                              80, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              300, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

/* ---------------------------------------------------------------------------------------------- */
/*                                         DANIELIB CONFIG                                        */
/* ---------------------------------------------------------------------------------------------- */

// STILL USED FOR DISTANCE RESETS
danielib::Beam left_beam(-90, -4.375, 2.5, distance_left);
danielib::Beam right_beam(90, 4.375, 2.5, distance_right);
danielib::Beam front_beam(0, -4, 4, distance_front);

// + offset is right or front, - offset is left or back
danielib::TrackerWheel vertical_tracker_danielib(vertical_rotation, 2, -0.5);
danielib::TrackerWheel horizontal_tracker_danielib(horizontal_rotation, 2, -2.4);
danielib::Inertial inertial(imu_1);

danielib::Localization mcl({left_beam, right_beam, front_beam});
danielib::Sensors sensors_danielib(vertical_tracker_danielib, horizontal_tracker_danielib, inertial, mcl);




danielib::PID angularPID(2.8, 0, 19, 0, 0, 0, 0);




danielib::PID linearPID(7.4, 0.09, 25, 0.75, 1, 70, 6);

danielib::PID mtpLinearPID(7.35, 0, 28.5, 0, 1.5, 90, 6);
danielib::PID mtpAngularPID(2.46, 0, 13.9, 0, 0, 0, 0);

danielib::PID swingAngularPID(6.2, 0.28, 61.8, 2, 0, 0, 0);

danielib::Drivetrain chassis_danielib(left_mg, right_mg, sensors_danielib, 11.3, 3.25, 450, linearPID, angularPID, mtpLinearPID, mtpAngularPID, swingAngularPID);
