#ifndef PLATFORM_H
#define PLATFORM_H

#include "../Utils/GlobalConstants.h"
#include "../TEng/Texture.h"

class Platform {

public:

    Platform(int x, int y, SDL_Renderer* renderer);
    void render() const;

    [[nodiscard]] SDL_Rect getPlatformRect() const { return platformRectangle; }

private:
    const int width = scale(TILE_SIZE);
    const int height = scale(17);

    SDL_Rect platformRectangle;
    Texture texture;

    SDL_Renderer* renderer;
};

#endif //PLATFORM_H
