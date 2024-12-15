#ifndef PLATFORM_H
#define PLATFORM_H

#include "GlobalConstants.h"
#include "Texture.h"

class Platform {

public:

    Platform(int x, int y, SDL_Renderer* renderer);
    void render() const;

    SDL_Rect &getPlatformRect() { return platformRectangle; }

private:
    const int width = scale(TILE_SIZE);
    const int height = scale(17);

    SDL_Rect platformRectangle;
    Texture texture;

    SDL_Renderer* renderer;
};

#endif //PLATFORM_H
