#pragma once

void DrivetrainControl();
void IntakeControl();
void LiftControl();
void ClawControl();

float getLiftPosition();
void moveLiftToPosition(float target, int timeout = 5000);