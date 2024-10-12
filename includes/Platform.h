#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL.h>

#include "GlobalConstants.h"
#include "Texture.h"


class Platform {

public:

    Platform(int x, int y, SDL_Renderer* renderer);
    void render();

    SDL_Rect &getPlatformRect() { return box; }
    void setPosition(int x, int y);

private:
    SDL_Rect box;
    Texture texture;

    const int width = scale(110);
    const int height = scale(17);

};

#endif //PLATFORM_H
