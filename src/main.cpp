#include "main.h" // IWYU pragma: keep

/* ---------------------------------------------------------------------------------------------- */
/*                                        GLOBAL VARIABLES                                        */
/* ---------------------------------------------------------------------------------------------- */

const bool skillsSlow = true;
const bool autoForDriver = false;

bool comp_started = false;
bool printing = false;
bool selecting = true;
int auton_index = 0;
float testAutonDuration = 0;

/* ---------------------------------------------------------------------------------------------- */
/*                                            AUTO LIST                                           */
/* ---------------------------------------------------------------------------------------------- */

void run_auton(int index) {
    // default auto to run when no auto is selected, also runs in test mode
    if (index == 0) {
        // auton_all_mid();
    }

    // // sawps
    // else if (index == 1)    auton_sawp_counter_nowing();
    // else if (index == 2)    auton_sawp_counter_wing();
    // else if (index == 3)    auton_sawp_standard();
    // else if (index == 4)    auton_sawp_low_mid();

    // // left sides
    // else if (index == 5)    auton_left_split();
    // else if (index == 6)    auton_left_4ball_loader();
    // else if (index == 7)    auton_left_4ball_stack();
    // // else if (index == 8)    auton_left_7ball();
    // else if (index == 9)    auton_left_7ball_counter();

    // // right sides
    // else if (index == 10)   auton_right_split();
    // else if (index == 11)   auton_right_4ball_loader();
    // else if (index == 12)   auton_right_4ball_stack();
    // else if (index == 13)   auton_right_7ball();
    // else if (index == 14)   auton_right_7ball_counter();

    // // misc autos
    // else if (index == 15)   auton_all_mid();
}

std::pair<std::string, std::string> get_auton_name(int index) {
    if (index == 0) return {"None", ""};

    // sawps
    else if (index == 1)    return {"Counter SAWP no wing", ""};
    else if (index == 2)    return {"Counter SAWP + Wing", ""};
    else if (index == 3)    return {"Standard SAWP", ""};
    else if (index == 4)    return {"Low goal SAWP", ""};

    // left sides
    else if (index == 5)    return {"Left split", "Loader setup"};
    else if (index == 6)    return {"Left 4 ball", "Loader"};
    else if (index == 7)    return {"Left 4 ball", "Stack"};
    // else if (index == 8)    return {"Left 7 ball", "Stack setup"};
    else if (index == 9)    return {"Left 7 counter", "Stack setup"};

    // right sides
    else if (index == 10)   return {"Right split", "Loader setup"};
    else if (index == 11)   return {"Right 4 ball", "Loader"};
    else if (index == 12)   return {"Right 4 ball", "Stack"};
    else if (index == 13)   return {"Right 7 ball", "Stack setup"};
    else if (index == 14)   return {"Right 7 counter", "Loader setup"};

    // misc autos
    else if (index == 15)   return {"All mid right", "Loader setup"};

    else return {"Invalid auto", ""};
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

void calibrate_all() {
    printf("calibrating \n");
    horizontal_rotation.reset_position();
    vertical_rotation.reset_position();
    chassis.calibrate();
    imu_1.set_heading(0);
}

void auton_selector() {
    pros::lcd::print(2, "A+X to test auto");
    pros::lcd::print(3, "Up+X for driver");
    while (selecting) {
        auto [line2, line3] = get_auton_name(auton_index);

        pros::lcd::print(0, "%d: %s", auton_index, line2.c_str());
        pros::lcd::print(1, "%s", line3.c_str());

        // print text to controller
        master.print(0, 0, "%d  ", auton_index);
        pros::delay(50);
        master.print(1, 0, "%s              ", line2.c_str());
        pros::delay(50);
        master.print(2, 0, "%s              ", line3.c_str());

        if (master.get_digital_new_press(DIGITAL_RIGHT)) {
            auton_index++;
        }
        if (master.get_digital_new_press(DIGITAL_LEFT)) {
            auton_index--;
        }
        if (auton_index < 0) auton_index = 15;
        if (auton_index > 15) auton_index = 0;

        pros::delay(50);
    }
}

void print_info() {
    int cycle = 0;
    while (printing) {
        // print pose
        auto pose = chassis.getPose();
        pros::lcd::print(0, "Position: (%.2f, %.2f, %.2f)", pose.x, pose.y, d_reduce_to_0_360(pose.theta));
        pros::lcd::print(1, "Selected Auto: %s", get_auton_name(auton_index).first.c_str());
        pros::lcd::print(2, "H: %.3f, V: %.3f", (float)horizontal_rotation.get_position()/100.0f, (float)vertical_rotation.get_position()/100.0f);
        
        /**/
        // print temps
        pros::lcd::print(3, "Temps");

        auto left_temps = left_mg.get_temperature_all();
        auto right_temps = right_mg.get_temperature_all();
        auto bottom_temps = bottom.get_temperature_all();
        auto top_temp = top.get_temperature();

        std::string left_status = "OK";
        if (left_temps[0] >= 50.0f || left_temps[1] >= 50.0f || left_temps[2] >= 50.0f) left_status = "WARM";
        if (left_temps[0] >= 60.0f || left_temps[1] >= 60.0f || left_temps[2] >= 60.0f) left_status = "1/4";
        if (left_temps[0] >= 65.0f || left_temps[1] >= 65.0f || left_temps[2] >= 65.0f) left_status = "1/8";
        if (left_temps[0] >= 70.0f || left_temps[1] >= 70.0f || left_temps[2] >= 70.0f) left_status = "OFF";
        std::string right_status = "OK";
        if (right_temps[0] >= 50.0f || right_temps[1] >= 50.0f || right_temps[2] >= 50.0f) right_status = "WARM";
        if (right_temps[0] >= 60.0f || right_temps[1] >= 60.0f || right_temps[2] >= 60.0f) right_status = "1/4";
        if (right_temps[0] >= 65.0f || right_temps[1] >= 65.0f || right_temps[2] >= 65.0f) right_status = "1/8";
        if (right_temps[0] >= 70.0f || right_temps[1] >= 70.0f || right_temps[2] >= 70.0f) right_status = "OFF";
        std::string bottom_status = "OK";
        if (bottom_temps[0] >= 50.0f || bottom_temps[1] >= 50.0f) bottom_status = "WARM";
        if (bottom_temps[0] >= 60.0f || bottom_temps[1] >= 60.0f) bottom_status = "1/4";
        if (bottom_temps[0] >= 65.0f || bottom_temps[1] >= 65.0f) bottom_status = "1/8";
        if (bottom_temps[0] >= 70.0f || bottom_temps[1] >= 70.0f) bottom_status = "OFF";
        std::string top_status = "OK";
        if (top_temp >= 50.0f) top_status = "WARM";
        if (top_temp >= 60.0f) top_status = "1/4";
        if (top_temp >= 65.0f) top_status = "1/8";
        if (top_temp >= 70.0f) top_status = "OFF";

        pros::lcd::print(4, "L: %.0f %.0f %.0f (%s)",
            left_temps[0], left_temps[1], left_temps[2], left_status.c_str()
        );
        pros::lcd::print(5, "R: %.0f %.0f %.0f (%s)",
            right_temps[0], right_temps[1], right_temps[2], right_status.c_str()
        );
        pros::lcd::print(6, "B: %.0f %.0f (%s) %.1f %.1f", 
            bottom_temps[0], bottom_temps[1], bottom_status.c_str(), bottom.get_power(0), bottom.get_power(1)
        );
        pros::lcd::print(7, "T: %.0f (%s) %.1f",
            top_temp, top_status.c_str(), top.get_power()
        );
        /**/

        // print to controller
        if (cycle % 5 == 0) {
            master.print(0, 0, "(%.1f, %.1f, %.1f)     ", pose.x, pose.y, d_reduce_to_0_360(pose.theta));
            //delay(50);
            //master.print(1, 0, "B: %.0f%%    ", battery::get_capacity());
        }

        cycle++;
        delay(50);
    }
}

void wait_for_bypass() {
    while (!comp_started) {
        if (!competition::is_connected() && master.get_digital(DIGITAL_X) && master.get_digital(DIGITAL_A)) {
            waitUntilCondition(!master.get_digital(DIGITAL_X) && !master.get_digital(DIGITAL_A));
            calibrate_all();
            printing = false;
            selecting = false;
            // delay(250);
            autonomous();
            return;
        } else if (comp_started || (master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_X))) {
            waitUntilCondition(!master.get_digital(DIGITAL_UP) && !master.get_digital(DIGITAL_X));
            comp_started = true;
            break;
        } else if (master.get_digital(DIGITAL_DOWN) && master.get_digital(DIGITAL_B)) {
            printing = false;
            selecting = false;
            delay(150);
            //pros::lcd::clear();
            calibrate_all();
            waitUntilCondition(!(master.get_digital(DIGITAL_DOWN) && master.get_digital(DIGITAL_B)));
        }
        pros::delay(10);
    }
}

/* ---------------------------------------------------------------------------------------------- */
/*                                           COMP TASKS                                           */
/* ---------------------------------------------------------------------------------------------- */

void initialize() {
    printf("init \n");

    // initialize devices
    pros::lcd::initialize();
    master.clear();
    imu_1.set_data_rate(5);
    horizontal_rotation.set_data_rate(5);
    vertical_rotation.set_data_rate(5);
    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    bottom.set_brake_mode(MotorBrake::brake);
    top.set_brake_mode(MotorBrake::brake);
    optical_top.set_integration_time(5);
    optical_top.set_led_pwm(0);

    // skills things
    // odom_lift.extend();
    calibrate_all();
    autonomous();

    // pros::Task selector(auton_selector);
    pros::Task bypass(wait_for_bypass);
}

void competition_initialize() {
    printf("comp start \n");
    comp_started = true;
    printing = true;
    selecting = false;

    pros::lcd::print(0, "Calibrating...");
    calibrate_all();
    pros::Task logger(print_info);
}

void autonomous() {
    printf("auto start \n");
    
    left_mg.set_brake_mode_all(MotorBrake::brake);
    right_mg.set_brake_mode_all(MotorBrake::brake);
    master.clear();
    printing = true;
    pros::Task logger(print_info);

    auton_none();
    /**
    // delay
    if (competition::is_connected()) {
        // run_auton(auton_index);
    } else {
        int startTime;
        bool finished = false;
        pros::Task test_auto ([&] {
            startTime = millis();
            run_auton(auton_index);
            // auton_skills();
            finished = true;
        });

        waitUntilCondition(millis() > startTime + 15000);
        // waitUntilCondition(millis() > startTime + 60000 || finished);
        if (!finished) {
            test_auto.remove();
            // chassis.stopAllMovements();
        }
        top.brake();
        bottom.brake();
        left_mg.brake();
        right_mg.brake();
    }
    /**/
    comp_started = true;
}

void opcontrol() {
    printf("driver start \n");

    // wait for comp start or bypass
    waitUntilCondition(/* (competition::is_connected() && !competition::is_disabled() && !competition::is_autonomous()) || */ comp_started);
    printf("driver enable \n");
    int matchStartTime = millis();
    comp_started = true;
    selecting = false;

    master.rumble("..");

    // if (autoForDriver) {
    //     left_mg.set_brake_mode_all(MotorBrake::brake);
    //     right_mg.set_brake_mode_all(MotorBrake::brake);

    //     // pros::Task skills(auton_skills);

    //     waitUntilCondition(master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_X));
    //     waitUntilCondition(!master.get_digital(DIGITAL_UP) && !master.get_digital(DIGITAL_X));

    //     // skills.remove();
    //     // chassis.stopAllMovements();
    //     top.brake();
    //     bottom.brake();
    //     left_mg.brake();
    //     right_mg.brake();
    // }

    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    bottom.set_brake_mode_all(MotorBrake::brake);
    top.set_brake_mode_all(MotorBrake::brake);
    odom_lift.extend();
    optical_top.set_led_pwm(0);
    master.clear();

    pros::Task d_drivetrain_control     (DrivetrainControl);
    pros::Task d_intake_control         (IntakeControl);
    pros::Task d_loader_control         (LoaderControl);
    pros::Task d_descore_wing_control   (DescoreWingControl);
    pros::Task d_mid_descore_control    (MidDescoreControl);
    pros::Task d_intake_raise_control   (IntakeRaiseControl);

    printing = false;
    pros::lcd::shutdown();
    delay(50);
    pros::lcd::initialize();
    delay(50);
    printing = true;
    pros::Task logger(print_info);

    while (true) { pros::delay(50); }
}

void disabled() {
    printf("disabled \n");
}