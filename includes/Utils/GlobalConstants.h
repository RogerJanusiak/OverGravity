#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

constexpr double SCALE_FACTOR = 1;
constexpr double UI_SCALE_FACTOR = 1.5*SCALE_FACTOR;
constexpr double PLAYER_UI_SCALE_FACTOR = 1.8*SCALE_FACTOR;

inline int scale(const double toScale) { return static_cast<int>(toScale*SCALE_FACTOR); }
inline int scaleUI(const double toScale) {  return static_cast<int>(toScale*UI_SCALE_FACTOR);  }
inline int scalePlayerUI(const double toScale) {  return static_cast<int>(toScale*PLAYER_UI_SCALE_FACTOR);  }

constexpr int numberOfEnemies = 5;

const int WINDOW_WIDTH = scale(1280);
const int WINDOW_HEIGHT = scale(720);

const int TILE_SIZE = 80;
const int TILE_SIZE_SCALED = scale(80);

const float ACCELERATION = scale(5000);
const float TERMINAL_VELOCITY = scale(1000);

constexpr int JOYSTICK_DEAD_ZONE = 18000;

#endif
