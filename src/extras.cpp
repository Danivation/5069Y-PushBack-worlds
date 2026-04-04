#include "main.h" // IWYU pragma: keep

// Extra functions
float DeadBand(float input, float width) {
    if (fabs(input) < width) {
        return 0;
    }
    return input;
}

float ToVolt(float percent) {
    return (percent*12.0/100.0);
}
