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
lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              11.3, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_rotation, lemlib::Omniwheel::NEW_2, -2.4);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation, lemlib::Omniwheel::NEW_2, -0.5);

// odometry settings
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu_1 // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(5.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              40, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);
