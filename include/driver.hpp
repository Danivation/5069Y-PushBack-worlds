#pragma once

void DrivetrainControl();
void IntakeControl();
void LiftControl();
void ClawControl();
void WristControl();

float getLiftPosition();
void moveLiftToPosition(float target, int timeout = 5000);