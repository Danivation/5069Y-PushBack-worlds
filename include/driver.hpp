#pragma once
#include "pros/rtos.hpp"

void DrivetrainControl();
void IntakeControl();
void LiftControl();
void ClawControl();
void WristControl();

float getWristPosition();
float getLiftPosition();
void startLiftWristPIDS();
void setLiftTo(float target);
void setWristTo(float target);

extern pros::Task* liftPIDTask;
extern pros::Task* wristPIDTask;


extern std::atomic<bool> lift_has_pid_control;
extern std::atomic<bool> wrist_has_pid_control;