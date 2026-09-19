#include "auton.hpp"
#include "danielib/exit.hpp" // IWYU pragma: keep
#include "main.h"

/* ---------------------------------------------------------------------------------------------- */
/*                                        GLOBAL VARIABLES                                        */
/* ---------------------------------------------------------------------------------------------- */

const bool skillsSlow = false;
const bool autoForDriver = false;

pros::Color WrongColor = Color::blue;
bool comp_started = false;
bool printing = false;
bool logging = false;
bool selecting = true;
int auton_index = 0;
float testAutonDuration = 0;

/* ---------------------------------------------------------------------------------------------- */
/*                                            AUTO LIST                                           */
/* ---------------------------------------------------------------------------------------------- */

void run_auton(int index) {
    // default auto to run when no auto is selected, also runs in test mode
    if (index == 0) {
        auton_one_stack_flower_mir();
    }

    if (index == 1) auton_wesley_stacks_mir();
    // else if (index == 2) auton_wesley_stacks();
    else if (index == 3) auton_one_stack_flower_mir();
    else if (index == 4) auton_one_stack_flower();
    else if (index == 4) auton_one_stack_flower_farpin_mir();


}

std::pair<std::string, std::string> get_auton_name(int index) {
    if (index == 0) return {"None", ""};
 
    // left sides
    else if (index == 1)    return {"Wesley stacks 4pin", "Far"};
    // else if (index == 2)    return {"Wesley stacks 4pin", "Close"};
    else if (index == 3)    return {"Flower close stack 4pin", "Far"};
    else if (index == 4)    return {"Flower close stack 4pin", "Close"};
    else if (index == 5)    return {"Flower far stack 4pin", "Far"};

    else return {"Invalid auto", ""};
}

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

void calibrate_all() {
    printf("calibrating \n");

    c_lemlib.calibrate(false);
    c_danielib.calibrate();
    c_danielib.startTracking();
    c_lemlib.setPose(0, 0, 0);
    c_danielib.setPose(0, 0);
    delay(500);
    master.rumble(".");
}

void auton_selector() {
    pros::lcd::print(2, "A+X to test auto");
    pros::lcd::print(3, "Up+X for driver");
    while (selecting) {
        auto [line2, line3] = get_auton_name(auton_index);
        std::string color = "";
        if (WrongColor == Color::blue) {
            color = "RED";
        } else if (WrongColor == Color::red) {
            color = "BLUE";
        }

        pros::lcd::print(0, "%d: %s", auton_index, line2.c_str());
        pros::lcd::print(1, "%s", line3.c_str());

        // print text to controller
        master.print(0, 0, "%d   %s   ", auton_index, color);
        pros::delay(25);
        master.print(1, 0, "%s        ", line2.c_str());
        pros::delay(25);
        master.print(2, 0, "%s        ", line3.c_str());
        pros::delay(25);

        if (master.get_digital_new_press(DIGITAL_RIGHT)) {
            auton_index++;
        }
        if (master.get_digital_new_press(DIGITAL_LEFT)) {
            auton_index--;
        }
        if (master.get_digital_new_press(DIGITAL_A)) {
            if (WrongColor == Color::red) {
                WrongColor = Color::blue;
            } else {
                WrongColor = Color::red;
            }
        }
        if (auton_index < 0) auton_index = 17;
        if (auton_index > 17) auton_index = 0;
    }
}

// Maps the highest temperature to its status string
std::string get_status(float max_temp) {
    if (max_temp >= 70.0f) return "OFF";
    if (max_temp >= 65.0f) return "1/8";
    if (max_temp >= 60.0f) return "1/4";
    if (max_temp >= 45.0f) return "WARM";
    return "OK";
}

// Helper overload for standard containers (vectors, arrays, etc.)
template <typename Container>
requires requires(Container c) { std::begin(c); std::end(c); }
std::string get_status(const Container& temps) {
    if (temps.empty()) return "OK";
    float max_temp = *std::max_element(temps.begin(), temps.end());
    return get_status(static_cast<float>(max_temp));
}

void print_info() {
    int cycle = 0;
    while (printing) {
        // print pose
        auto pose = c_lemlib.getPose();
        pros::lcd::print(0, "Position: (%.2f, %.2f, %.2f)", pose.x, pose.y, d_reduce_to_0_360(pose.theta));
        pros::lcd::print(1, "Selected Auto: %s", get_auton_name(auton_index).first.c_str());
        pros::lcd::print(2, "H: %.3f, V: %.3f", (float)horizontal_rotation.get_position()/100.0f, (float)vertical_rotation.get_position()/100.0f);
        
        /**/
        // print temps
        pros::lcd::print(3, "Temps");

        auto left_temps  = left_mg.get_temperature_all();
        auto right_temps = right_mg.get_temperature_all();
        auto lift_temps  = lift.get_temperature_all();
        auto wrist_temp  = wrist.get_temperature();
        auto intake_temp = intake.get_temperature();
        auto cone_temp   = cone.get_temperature();

        // Evaluate statuses directly via helper overloads
        std::string left_status   = get_status(left_temps);
        std::string right_status  = get_status(right_temps);
        std::string lift_status   = get_status(lift_temps);
        std::string wrist_status  = get_status(wrist_temp);
        std::string intake_status = get_status(intake_temp);
        std::string cone_status   = get_status(cone_temp);

        // LCD Printing
        pros::lcd::print(4, "L: %.0f %.0f %.0f (%s)",
            left_temps[0], left_temps[1], left_temps[2], left_status.c_str());

        pros::lcd::print(5, "R: %.0f %.0f %.0f (%s)",
            right_temps[0], right_temps[1], right_temps[2], right_status.c_str());

        pros::lcd::print(6, "Lift: %.0f %.0f (%s)    Wrist: %.0f (%s)", 
            lift_temps[0], lift_temps[1], lift_status.c_str(), wrist_temp, wrist_status.c_str());

        pros::lcd::print(7, "Intake: %.0f (%s)    Cone: %.0f (%s)",
            intake_temp, intake_status.c_str(), cone_temp, cone_status.c_str());

        // print to controller
        if (cycle % 5 == 0) {
            master.print(0, 0, "(%.1f, %.1f, %.1f)    ", pose.x, pose.y, d_reduce_to_0_360(pose.theta));
            delay(50);
            master.print(1, 0, "LP: %.2f   WP: %.2f", getLiftPosition(), getWristPosition());
            delay(50);
            master.print(2, 0, "Time: %.2f    ", testAutonDuration);
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
            autonomous();
            return;
        } else if (comp_started || (master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_X))) {
            waitUntilCondition(!master.get_digital(DIGITAL_UP) && !master.get_digital(DIGITAL_X));
            comp_started = true;
            break;
        } else if (master.get_digital(DIGITAL_DOWN) && master.get_digital(DIGITAL_B)) {
            waitUntilCondition(!(master.get_digital(DIGITAL_DOWN) && master.get_digital(DIGITAL_B)));
            printing = false;
            selecting = false;
            calibrate_all();
        }
        pros::delay(10);
    }
}

void logger() {
    FILE* log_pose = fopen("/usd/log_pose_d.txt", "w");
    FILE* log_horiz = fopen("/usd/log_horiz_d.txt", "w");
    FILE* log_vert = fopen("/usd/log_vert_d.txt", "w");
    while (true) {
        auto pose = c_lemlib.getPose();
        if (log_pose) fprintf(log_pose, "(%.3f,%.3f),", pose.x, pose.y);
        if (log_horiz) fprintf(log_horiz, "(%d,%.2f),", pros::millis(), (float)horizontal_rotation.get_position()/100.0f);
        if (log_vert) fprintf(log_vert, "(%d,%.2f),", pros::millis(), (float)vertical_rotation.get_position()/100.0f);
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

    lift_rot.set_data_rate(5);
    wrist_rot.set_data_rate(5);

    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    lift.set_brake_mode_all(MotorBrake::hold);
    wrist.set_brake_mode(MotorBrake::hold);
    intake.set_brake_mode(MotorBrake::brake);
    cone.set_brake_mode(MotorBrake::brake);

    // skills things
    // calibrate_all();
    // autonomous();

    pros::Task selector(auton_selector);
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
    optical_top.set_led_pwm(100);
    printing = true;
    pros::Task printer (print_info);
    pros::Task a_pids (startLiftWristPIDS);
    pros::Task a_log (logger);

    if (competition::is_connected()) {
        run_auton(auton_index);
    } else {
        int startTime = millis();
        run_auton(auton_index);
        int endTime = millis();
        left_mg.brake();
        right_mg.brake();
        testAutonDuration = (float)(endTime - startTime)/(1000.0f);
    }
    comp_started = true;
    a_log.remove();
}

void opcontrol() {
    printf("driver start \n");

    // wait for comp start or bypass
    waitUntilCondition(comp_started);
    printf("driver enable \n");
    int matchStartTime = millis();
    comp_started = true;
    selecting = false;
    master.rumble("..");
    master.clear();

    // SETUP MOTORS

    left_mg.set_brake_mode_all(MotorBrake::coast);
    right_mg.set_brake_mode_all(MotorBrake::coast);
    lift.set_brake_mode_all(MotorBrake::hold);
    wrist.set_brake_mode(MotorBrake::hold);
    intake.set_brake_mode(MotorBrake::brake);
    cone.set_brake_mode(MotorBrake::brake);

    setWristTo(getWristPosition());
    setLiftTo(getLiftPosition());


    pros::Task d_pids                   (startLiftWristPIDS);
    pros::Task d_drivetrain_control     (DrivetrainControl);
    pros::Task d_intake_control         (IntakeControl);
    pros::Task d_wrist_control          (WristControl);
    pros::Task d_lift_control           (LiftControl);

    printing = false;
    pros::lcd::shutdown();
    delay(100);
    pros::lcd::initialize();
    delay(100);
    printing = true;
    pros::Task printer(print_info);

    while (true) { pros::delay(50); }
}

void disabled() {
    printf("disabled \n");
}