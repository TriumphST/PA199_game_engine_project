#include "Helper.h"

# define M_PI           3.14159265358979323846

float Helper::toRadians(float x) {
    return 2 * M_PI * (x / 360);
}