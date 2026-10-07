#include "driver.hpp"
#include "main.h"

/* ---------------------------------------------------------------------------------------------- */
/*                                       BUTTON ASSIGNMENTS                                       */
/* ---------------------------------------------------------------------------------------------- */

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)

#define INTAKE                  DIGITAL_R1
#define OUTTAKE                 DIGITAL_R2

#define LIFT_UP                 DIGITAL_L1
#define LIFT_DOWN               DIGITAL_L2

#define WRIST_UP_MANUAL         DIGITAL_UP
#define WRIST_DOWN_MANUAL       DIGITAL_LEFT

#define MATCHLOAD_MACRO         DIGITAL_DOWN
#define INTAKE_MACRO            DIGITAL_B
#define SCORING_MACRO           DIGITAL_Y
#define OUTTAKE_MACRO           DIGITAL_RIGHT

/* ---------------------------------------------------------------------------------------------- */
/*                                          MACRO TUNINGS                                         */
/* ---------------------------------------------------------------------------------------------- */


std::atomic<bool> driving = true;
std::atomic<bool> intake_control = true;
std::atomic<bool> wrist_control = true;
std::atomic<bool> lift_has_pid_control = false;
std::atomic<bool> wrist_has_pid_control = false;
bool readyForSecondPress = false;
bool inMatchLoadHoldPosition = false;
bool inPinPosition = false;
bool inScoringPosition = false;

float liftOffset = -4;

void intakePin() {
    setWristTo(-18);
    setLiftTo(17.3+liftOffset);
}

void intakeCup() {
    setWristTo(21);
    setLiftTo(17.8+liftOffset);
}

void holdVertical() {
    setWristTo(117);
}

void holdFlat() {
    setWristTo(117);
}

void holdStack() {
    holdVertical();
    // if stack is in intake OR just came from match loader
    if (getLiftPosition() < 22+liftOffset) setLiftTo(30+liftOffset);
}

void flipOut() {
    holdStack();
}

void hover() {
    cone.brake();

    // flat wrist
    setLiftTo(23.3+liftOffset);
    setWristTo(119);

    // // wrist pointed up
    // setLiftTo(15.5);
    // setWristTo(126);
}

void load() {
    cone.brake();

    // // wrist pointed up
    // setLiftTo(15.3);
    // setWristTo(126);

    // flat wrist load
    setLiftTo(19.5+liftOffset);
    setWristTo(117);
}

void grabFlat() {
    intake_control = false;
    cone.move(127);
    setWristTo(110);
    delay(50);
    setLiftTo(-10+liftOffset);
    delay(400);
    setLiftTo(getLiftPosition());
    lift_has_pid_control = false;
    // setLiftTo(15);
    setWristTo(88);
    delay(100);
    intake_control = true;
}

void grab() {
    intake_control = false;
    cone.move(127);
    setWristTo(110);
    delay(50);
    setLiftTo(-10+liftOffset);
    delay(300);
    setWristTo(125);
    delay(200);
    setLiftTo(9.5+liftOffset);
    setWristTo(136);
    intake_control = true;
}

void score() {
    lift_has_pid_control = false;
    cone.move(127);
    lift.move(-55);
    delay(550);
    lift_has_pid_control = true;
    cone.move(-127);
    holdVertical();
    setLiftTo(getLiftPosition() + 25);
    delay(300);
}

// ALLIANCE GOAL HEIGHTS
float allianceBase = 20+liftOffset;

void a0() {
    setWristTo(145);
    setLiftTo(16);
}

void a1() {
    setWristTo(115);
    setLiftTo(allianceBase+15);
}

void a2() {
    setWristTo(119);
    setLiftTo(allianceBase+15+15);
}

void a3() {
    setWristTo(121);
    setLiftTo(allianceBase+15+15+15);
}

void a4() {
    setWristTo(123);
    setLiftTo(allianceBase+15+15+15+15);
}

void a5() {
    setWristTo(123);
    setLiftTo(allianceBase+15+15+15+15+15);
}

// NEUTRAL GOAL HEIGHTS
float neutralBase = 24+liftOffset;

void n0() {
    setWristTo(125);
    setLiftTo(neutralBase);
}

void n1() {
    setWristTo(115);
    setLiftTo(neutralBase+15);
}

void n2() {
    setWristTo(117);
    setLiftTo(neutralBase+15+15);
}

void n3() {
    setWristTo(119);
    setLiftTo(neutralBase+15+15+15);
}

void n4() {
    setWristTo(121);
    setLiftTo(neutralBase+15+15+15+15);
}

void n5() {
    setWristTo(123);
    setLiftTo(neutralBase+15+15+15+15+15);
}

// CENTER GOAL HEIGHTS
float centerBase = 28+liftOffset;

void c0() {
    setWristTo(125);
    setLiftTo(centerBase);
}

void c1() {
    setWristTo(117);
    setLiftTo(centerBase+15);
}

void c2() {
    setWristTo(119);
    setLiftTo(centerBase+15+15);
}

void c3() {
    setWristTo(121);
    setLiftTo(centerBase+15+15+15);
}

void c4() {
    setWristTo(123);
    setLiftTo(centerBase+15+15+15+15);
}














/* ---------------------------------------------------------------------------------------------- */
/*                                    INTAKE CUP DETECTION TASK                                   */
/* ---------------------------------------------------------------------------------------------- */

void cupTask() {
    // CHECK FOR INTAKE POSITION FIRST
    pros::Task autoCupTask {[&] {
        while (true) {
            
            // first distance check
            if (inPinPosition && pin_dist.get_distance() < 150) {

            // delay 200 for pin
            delay(200);

            // second distance check
            if (pin_dist.get_distance() < 150) {

            // delay 100, check again
            delay(100);

            // third distance check
            if (pin_dist.get_distance() < 150) {
                inPinPosition = false;
                intakeCup();
            }
            }
            }
        delay(10);
        }
    }};
}


/* ---------------------------------------------------------------------------------------------- */
/*                                       LIFT + WRIST MACROS                                      */
/* ---------------------------------------------------------------------------------------------- */

void WristControl() {
    cupTask();

    /* ---------------------------------------------------------------------------------------------- */
    /*                                             MACROS                                             */
    /* ---------------------------------------------------------------------------------------------- */

    while (true) {

        // MATCHLOAD POSITIONS
        if (master.get_digital_new_press(MATCHLOAD_MACRO)) {
            inScoringPosition = false;

            if (!readyForSecondPress) {
                // LOAD HEIGHT
                inPinPosition = false;
                cone.brake();
                load();

                inMatchLoadHoldPosition = false;
                readyForSecondPress = true;

            } 
            else {
                // LOAD DOWN
                inPinPosition = false;
                grab();

                readyForSecondPress = false;
                inMatchLoadHoldPosition = true;
            }

        } 
        // INTAKE PIN POSITION
        else if (master.get_digital_new_press(INTAKE_MACRO)) {

            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            inScoringPosition = false;
            intakePin();

            inPinPosition = true;

        } 
        // HOLD STACK VERTICAL POSITION
        else if (master.get_digital_new_press(SCORING_MACRO)) {

            readyForSecondPress = false;
            inPinPosition = false;
            inMatchLoadHoldPosition = false;
            inScoringPosition = true;

            holdVertical();

        } 

        /* ---------------------------------------------------------------------------------------------- */
        /*                                      MANUAL WRIST CONTROL                                      */
        /* ---------------------------------------------------------------------------------------------- */

        // MANUAL WRIST UP
        else if (master.get_digital(WRIST_UP_MANUAL)) {
            inPinPosition = false;
            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            wrist_has_pid_control = false;
            inScoringPosition = false;


            delay(10);
            wrist.move(60);


            waitUntilCondition(!master.get_digital(WRIST_UP_MANUAL));
            wrist.brake();
        } 
        // MANUAL WRIST DOWN
        else if (master.get_digital(WRIST_DOWN_MANUAL)) {
            inPinPosition = false;
            readyForSecondPress = false;
            inMatchLoadHoldPosition = false;
            wrist_has_pid_control = false;
            inScoringPosition = false;


            delay(10);
            wrist.move(-60);


            waitUntilCondition(!master.get_digital(WRIST_DOWN_MANUAL));
            wrist.brake();
        } 
        else {
            // wrist.brake();
        }
        delay(10);
    }
}



/* ---------------------------------------------------------------------------------------------- */
/*                                          LIFT CONTROL                                          */
/* ---------------------------------------------------------------------------------------------- */



void LiftControl() {
    while (true) {
        if (master.get_digital(LIFT_UP)) {
            lift_has_pid_control = false;
            delay(10);


            if (getLiftPosition() < 22+liftOffset) holdVertical();
            lift.move(127);


            waitUntilCondition(!master.get_digital(LIFT_UP));
            lift.brake();
            inMatchLoadHoldPosition = false;
            readyForSecondPress = false;
        }
        else if (master.get_digital(LIFT_DOWN)) {
            lift_has_pid_control = false;
            delay(10);


            lift.move(-90);


            waitUntilCondition(!master.get_digital(LIFT_DOWN));
            lift.brake();
            inMatchLoadHoldPosition = false;
            readyForSecondPress = false;
        }
        else {
            // lift.brake();
        }
        delay(10);
    }
}



/* ---------------------------------------------------------------------------------------------- */
/*                                         INTAKE CONTROL                                         */
/* ---------------------------------------------------------------------------------------------- */



void IntakeControl() {
    while (true) {
        if (intake_control) {
            if (master.get_digital(INTAKE)) {
                cone.move(127);
                if (getLiftPosition() < 30) intake.move(127);
                else intake.brake();

                
                // waitUntilCondition(!master.get_digital(INTAKE) || getLiftPosition() < 30);
                // intake.brake();
                // cone.brake();
            }
            else if (master.get_digital(OUTTAKE)) {
                cone.move(-127);
                if (getLiftPosition() < 30) intake.move(-127);
                else intake.brake();


                // waitUntilCondition(!master.get_digital(OUTTAKE));
                // intake.brake();
                // cone.brake();
            }
            else if (master.get_digital(OUTTAKE_MACRO)) {
                intake.move(-127);

                // waitUntilCondition(!master.get_digital(OUTTAKE_MACRO));
                // intake.brake();
            } else {
                intake.brake();
                cone.brake();
            }
        }
        delay(10);
    }
}



/* ---------------------------------------------------------------------------------------------- */
/*                                       DRIVETRAIN CONTROL                                       */
/* ---------------------------------------------------------------------------------------------- */



void DrivetrainControl() {
    float throttle;
    float turn;
    while (true) {
        if (driving) {
            throttle = DeadBand(THROTTLE_AXIS, 2);
            turn = DeadBand(TURN_AXIS, 2);
            left_mg.move(throttle + turn);
            right_mg.move(throttle - turn);
            delay(10);
        }
    }
}






/* ---------------------------------------------------------------------------------------------- */
/*                                         LIFT/WRIST PIDS                                        */
/* ---------------------------------------------------------------------------------------------- */


pros::Task* liftPIDTask = nullptr;
pros::Task* wristPIDTask = nullptr;
float liftCurrentTarget = 0;
float wristCurrentTarget = 0;
float getLiftPosition() {
    return (float)(lift_rot.get_position())/100.0f;
}
float getWristPosition() {
    return (float)(wrist_rot.get_position())/300.0f;
}
void setLiftTo(float target) {
    lift_has_pid_control = true;
    liftCurrentTarget = target;
}
void setWristTo(float target) {
    wrist_has_pid_control = true;
    wristCurrentTarget = target;
}



void startLiftWristPIDS() {

    liftPIDTask = new pros::Task {[&] {
        const int startTime = pros::millis();
        danielib::ExitCondition liftExit(liftPID.exitRange, liftPID.exitTime);

        float power = 0;
        float currentPosition = getLiftPosition();
        float error = 0;

        liftPID.reset();
        liftExit.reset();

        std::uint32_t time = pros::millis();
        while (/* pros::millis() < startTime + timeout && !liftExit.isDone() */ true) {
            currentPosition = getLiftPosition();
            error = liftCurrentTarget - currentPosition;
            power = liftPID.update(error);
            liftExit.update(error);

            // move motors
            if (lift_has_pid_control) lift.move(power);

            // delay
            pros::Task::delay_until(&time, 10);
        }

        lift.brake();
    }};



    wristPIDTask = new pros::Task {[&] {
        const int startTime = pros::millis();
        danielib::ExitCondition wristExit(wristPID.exitRange, wristPID.exitTime);

        float power = 0;
        float currentPosition = getWristPosition();
        float error = 0;

        wristPID.reset();
        wristExit.reset();

        std::uint32_t time = pros::millis();
        while (/* pros::millis() < startTime + timeout && !wristExit.isDone() */ true) {
            currentPosition = getWristPosition();
            error = wristCurrentTarget - currentPosition;
            power = wristPID.update(error);
            wristExit.update(error);

            // move motors
            if (wrist_has_pid_control) wrist.move(power);

            // delay
            pros::Task::delay_until(&time, 10);
        }

        wrist.brake();
    }};
}
