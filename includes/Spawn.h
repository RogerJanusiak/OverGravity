#ifndef SPAWN_H
#define SPAWN_H
#include <SDL_rect.h>

#include "GlobalConstants.h"


class Spawn {

public:

    Spawn() = default;
    Spawn(const int x, const int y, const int w, const int h, const int _type) { init(x, y, w, h); type = _type; }
    void init(const int x, const int y, const int w, const int h) { rect.x = x; rect.y = y; rect.w = w; rect.h = h; }

    int getX() const { return rect.x; }
    int getY() const { return rect.y; }

    void setPosition(int x, int y) { rect.x = x; rect.y = y; }

    void setOccupied(const bool occupy) { occupied = occupy; }
    bool getOccupied() const { return occupied; }

    int getSpawnType() const { return type; }

    SDL_Rect& getRect() { return rect; }
    const SDL_Rect *getRectP() const { return &rect; }

private:

    SDL_Rect rect;
    bool occupied = false;
    int type = 0;

};

#endif //SPAWN_H
