#ifndef SPAWN_H
#define SPAWN_H
#include <SDL_rect.h>

#include "GlobalConstants.h"


class Spawn {

public:

    Spawn() = default;
    Spawn(const int x, const int y, const int w, const int h) { init(x, y, w, h); }
    void init(const int x, const int y, const int w, const int h) { rect.x = x; rect.y = y; rect.w = w; rect.h = h; }

    int getX() const { return rect.x; }
    int getY() const { return rect.y; }

    void setOccupied(const bool occupy) { occupied = occupy; }
    bool getOccupied() const { return occupied; }

    SDL_Rect& getRect() { return rect; }
    const SDL_Rect *getRectP() const { return &rect; }

private:

    SDL_Rect rect;
    bool occupied = false;

};

#endif //SPAWN_H
