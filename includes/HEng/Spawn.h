#ifndef SPAWN_H
#define SPAWN_H
#include <SDL_rect.h>

#include "../Utils/GlobalConstants.h"


class Spawn {

public:

    Spawn() = default;
    Spawn(const int x, const int y, const int w, const int h, const int _type) { init(x, y, w, h); type = _type; }
    void init(const int x, const int y, const int w, const int h) { rect.x = x; rect.y = y; rect.w = w; rect.h = h; }

    int getX() const { return rect.x; }
    int getY() const { return rect.y; }

    void setPosition(int x, int y) { rect.x = x; rect.y = y; }

    void setOccupied(const bool occupy) { occupied = occupy; }
    [[nodiscard]] bool getOccupied() const { return occupied; }

    void setOnScreen(const bool isOnScreen) { onScreen = isOnScreen; }
    [[nodiscard]] bool isOnScreen() const { return onScreen; }

    int getSpawnType() const { return type; }

    SDL_Rect& getRect() { return rect; }
    const SDL_Rect *getRectP() const { return &rect; }

    void render(SDL_Renderer* renderer) {
        type == 1 ? SDL_SetRenderDrawColor(renderer, 0, 105, 0, 100) : SDL_SetRenderDrawColor(renderer, 105, 0, 0, 100);
        SDL_RenderFillRect(renderer, &rect);
    }

private:

    SDL_Rect rect;
    bool occupied = false;
    bool onScreen = true;
    int type = 0;

};

#endif //SPAWN_H
