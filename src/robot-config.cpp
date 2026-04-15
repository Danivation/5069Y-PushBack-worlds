#include "main.h"


/* ---------------------------------------------------------------------------------------------- */
/*                                          DEVICE PORTS                                          */
/* ---------------------------------------------------------------------------------------------- */

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({13, -12, -11}, pros::MotorGears::blue);
pros::MotorGroup right_mg({-18, 19, 20}, pros::MotorGears::blue);

pros::MotorGroup bottom({8, 9}, pros::MotorGears::rpm_200);
pros::Motor top(1, pros::MotorGears::rpm_200);

CustomImu imu_1(15, 360.0f/356.6f);
// pros::Imu imu_2(22); // unused
pros::Rotation vertical_rotation(-17);
pros::Rotation horizontal_rotation(14);

pros::Optical optical_top(7);
pros::Distance distance_front(2);
pros::Distance distance_left(3);
pros::Distance distance_right(10);

pros::adi::Pneumatics odom_lift('B', false);
pros::adi::Pneumatics hood('C', false);
pros::adi::Pneumatics wing('D', false);
pros::adi::Pneumatics intake_raise('E', false);
pros::adi::Pneumatics mid_descore('F', false);
pros::adi::Pneumatics loader('G', false);

/* ---------------------------------------------------------------------------------------------- */
/*                                              PIDS                                              */
/* ---------------------------------------------------------------------------------------------- */

// LEMLIB LINEAR PID - very good for mtp, no tip
lemlib::ControllerSettings lateral_controller(7.5, 0, 33, 0, 1, 100, 2.5, 300, 7.5);

// LEMLIB ANGULAR PID - good for mtp, little end turning, but may not be good for normal turns
lemlib::ControllerSettings angular_controller(5.7, 0, 42, 0, 1, 85, 3, 300, 0);

// DANIELIB ANGULAR PID - tuned for normal turns
danielib::PID angularPID(2.83, 0.12, 18.7, 4, 1.5, 120, 0);
danielib::PID swingAngularPID(5.9, 0.2, 40, 4, 2, 160, 0);

// DANIELIB LINEAR PID - tuned for straight drives
danielib::PID linearPID(7.5, 0, 33, 0, 1.5, 150, 6.5);

// DANIELIB MTP - somewhat tuned, but not very optimal
danielib::PID mtpLinearPID(7.35, 0, 28.5, 0, 1.5, 90, 7.5);
danielib::PID mtpAngularPID(2.95, 0, 18, 0, 0, 0, 0); // tuned?

/* ---------------------------------------------------------------------------------------------- */
/*                                          LEMLIB CONFIG                                         */
/* ---------------------------------------------------------------------------------------------- */

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 11.3, lemlib::Omniwheel::NEW_325, 450, 2);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation, 2, -0.5);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_rotation, 2, -2.4);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, &horizontal_tracking_wheel, nullptr, &imu_1);

// create the chassis
lemlib::Chassis c_lemlib(drivetrain, lateral_controller, angular_controller, sensors);

/* ---------------------------------------------------------------------------------------------- */
/*                                         DANIELIB CONFIG                                        */
/* ---------------------------------------------------------------------------------------------- */

// STILL USED FOR DISTANCE RESETS
danielib::Beam left_beam(-90, -4.5, -2.25, distance_left);
danielib::Beam right_beam(90, 4.5, -2.25, distance_right);
danielib::Beam front_beam(0, -4.7, 3.7, distance_front);

// + offset is right or front, - offset is left or back
danielib::TrackerWheel vertical_tracker_danielib(vertical_rotation, 2, -0.5);
danielib::TrackerWheel horizontal_tracker_danielib(horizontal_rotation, 2, -2.4);
danielib::Inertial inertial(imu_1);

danielib::Localization mcl({left_beam, right_beam, front_beam});
danielib::Sensors sensors_danielib(vertical_tracker_danielib, horizontal_tracker_danielib, inertial, mcl);

danielib::Drivetrain c_danielib(left_mg, right_mg, sensors_danielib, 11.3, 3.25, 450, linearPID, angularPID, mtpLinearPID, mtpAngularPID, swingAngularPID);
