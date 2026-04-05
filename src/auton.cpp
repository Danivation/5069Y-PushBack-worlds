#include "main.h"
using namespace pros;

/* ---------------------------------------------------------------------------------------------- */
/*                                             HELPERS                                            */
/* ---------------------------------------------------------------------------------------------- */

constexpr double operator""_tiles(long double value) {
    return value * 23.622;
}
constexpr double operator""_tiles(unsigned long long value) {
    return static_cast<double>(value) * 23.622;
}
void store() {
    hood.retract();
    bottom.move(127);
    top.move(127);
}
void score() {
    hood.extend();
    bottom.move(127);
    top.move(127);
}
void score_mid() {
    hood.retract();
    bottom.move(127);
    top.move(-127);
}
void stop() {
    bottom.brake();
    top.brake();
}
void outtake() {
    hood.retract();
    bottom.move(-127);
    top.move(-127);
}

/* ---------------------------------------------------------------------------------------------- */
/*                                        NONE (TEST AUTO)                                        */
/* ---------------------------------------------------------------------------------------------- */

void auton_none() {
    chassis.setPose(0, 0, 0);
    chassis_danielib.setPose(0, 0);

    // chassis_danielib.turnToHeading(90, 5000);
    // chassis.turnToHeading(140, 5000, {}, false); 

    chassis.moveToPoint(0, 24, 10000, {}, false);
}
