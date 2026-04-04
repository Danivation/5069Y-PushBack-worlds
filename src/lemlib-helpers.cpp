#include "danielib/danielib.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep

void lemlibDistReset(std::initializer_list<danielib::Beam*> beams, float xChange, float yChange) {
    float sumX = 0;
    float sumY = 0;
    int countX = 0;
    int countY = 0;

    // get pose once
    auto startPose = chassis.getPose();

    // loop through all beams
    for (danielib::Beam* beamPtr : beams) {
        if (!beamPtr) continue;  // skip null pointers
        
        danielib::Beam& beam = *beamPtr;
        
        // update beam distance (updates the actual beam object)
        beam.update();

        // skip bad readings
        float wallDistance = d_toInches(beam.distance);
        if (wallDistance <= 0) continue;

        // find global beam angle and position
        float robotAngle = d_fixRadians(d_toRadians(startPose.theta));
        float sinRobotAngle = std::sin(robotAngle);
        float cosRobotAngle = std::cos(robotAngle);

        // calculate beam angle and position using offset
        float beamAngle = d_fixRadians(d_toRadians(startPose.theta + beam.angleOffset));
        float beamX = startPose.x + beam.yOffset * cosRobotAngle + beam.xOffset * sinRobotAngle;
        float beamY = startPose.y + beam.yOffset * sinRobotAngle - beam.xOffset * cosRobotAngle;

        float sinBeamAngle = std::sin(beamAngle);
        float cosBeamAngle = std::cos(beamAngle);

        // calculate x and y positions of the wall based on beam distance
        // this is basically where it thinks the wall is based on that beam
        // (wallX, wallY) is the point on the wall that the beam is hitting
        float wallX = beamX + wallDistance * cosBeamAngle;
        float wallY = beamY + wallDistance * sinBeamAngle;

        // we need to filter this data to only use x and y positions that are actually on a wall
        // a beam facing one wall will not tell you anything useful about the other wall

        // Normalize angle to [0, 2π)
        float normalizedAngle = d_reduce_radians(beamAngle);

        float tolerance = d_toRadians(10);

        // Check if beam is aligned with east/west walls (pointing near 0° or 180°)
        bool pointingEast = (normalizedAngle < tolerance) || (normalizedAngle > 2 * M_PI - tolerance);
        bool pointingWest = (normalizedAngle > M_PI - tolerance) && (normalizedAngle < M_PI + tolerance);

        // Check if beam is aligned with north/south walls (pointing near 90° or 270°)
        bool pointingNorth = (normalizedAngle > M_PI_2 - tolerance) && (normalizedAngle < M_PI_2 + tolerance);
        bool pointingSouth = (normalizedAngle > 3 * M_PI_2 - tolerance) && (normalizedAngle < 3 * M_PI_2 + tolerance);
        
        if (pointingEast || pointingWest) {
            // Known wall position
            float knownWallX = pointingEast ? 70.65 : -70.65;
            
            // Calculate where the beam sensor must be
            float beamX = knownWallX - wallDistance * cosBeamAngle;
            
            // Now calculate robot center from beam position
            // Reverse the offset transformation
            float robotX = beamX - (beam.yOffset * cosRobotAngle + beam.xOffset * sinRobotAngle);
            
            sumX += robotX;
            countX++;
        }

        if (pointingNorth || pointingSouth) {
            // Known wall position
            float knownWallY = pointingNorth ? 70.65 : -70.65;
            
            // Calculate where the beam sensor must be
            float beamY = knownWallY - wallDistance * sinBeamAngle;
            
            // Now calculate robot center from beam position
            // Reverse the offset transformation
            float robotY = beamY - (beam.yOffset * sinRobotAngle - beam.xOffset * cosRobotAngle);
            
            sumY += robotY;
            countY++;
        }
    }

    float newX = (countX > 0) ? (sumX / countX) : startPose.x;
    float newY = (countY > 0) ? (sumY / countY) : startPose.y;
    float changeX = std::abs(newX - startPose.x);
    float changeY = std::abs(newY - startPose.y);

    // sets x and y based on averages of valid readings independently, keeps old value if no valid readings
    // if only x readings are valid, only x is changed (same for y)
    if (changeX < xChange && changeY < yChange && std::abs(newX) < 80 && std::abs(newY) < 80) {
        chassis.setPose({newX, newY, startPose.theta});
    }
}