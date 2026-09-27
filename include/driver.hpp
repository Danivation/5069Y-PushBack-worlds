#pragma once
#include "pros/rtos.hpp"



void intake_pin();
void intake_cup();
void hold_vertical();
void hold_stack();
void load();
void grab();
void score();
void a0();
void a1();
void a2();
void a3();
void a4();
void a5();
void n0();
void n1();
void n2();
void n3();
void n4();
void n5();



void cup_task();


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

extern bool inPinPosition;