#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL.h>
#include <SDL_image.h>

#include "GlobalConstants.h"
#include "Texture.h"


class Platform {

public:

    Platform(int x, int y, SDL_Renderer* renderer);
    void render();

    SDL_Rect &getPlatformRect() { return box; }

private:
    SDL_Rect box;
    Texture texture;

    const int width = 110*SCALE_FACTOR;
    const int height = 17*SCALE_FACTOR;

};



#endif //PLATFORM_H
