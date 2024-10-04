#ifndef SPAWN_H
#define SPAWN_H
#include <SDL_rect.h>

#include "GlobalConstants.h"


class Spawn {

public:

    Spawn() = default;
    Spawn(int x, int y, int w, int h) { init(x, y-25, w, h); }
    void init(int x, int y, int w, int h) { rect.x = x*SCALE_FACTOR; rect.y = y*SCALE_FACTOR; rect.w = w*SCALE_FACTOR; rect.h = h*SCALE_FACTOR; }

    int getX() const { return rect.x; }
    int getY() const { return rect.y; }

    void setOccupied(const bool occupy) { occupied = occupy; }
    bool getOccupied() const { return occupied; }

    SDL_Rect& getRect() { return rect; }
    const SDL_Rect *getRectP() { return &rect; }

private:

    SDL_Rect rect;
    bool occupied = false;

};

#endif //SPAWN_H
