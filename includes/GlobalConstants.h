#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

constexpr double SCALE_FACTOR = 2  ;

inline double scale(const double toScale) {
    return toScale*SCALE_FACTOR;
}

const int WINDOW_WIDTH = scale(800);
const int WINDOW_HEIGHT = scale(450);
const int ACCELERATION = scale(7000);

constexpr int JOYSTICK_DEAD_ZONE = 18000;

#endif
