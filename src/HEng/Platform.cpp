#include "../../includes/HEng/Platform.h"

Platform::Platform(const int x, const int y, SDL_Renderer* renderer) : renderer(renderer) {
    platformRectangle.x = scale(x);
    platformRectangle.y = scale(y);
    platformRectangle.w = width;
    platformRectangle.h = height;
    texture.setup(width,height,renderer);

    if(!texture.loadFromFile("platform.png")) {
        SDL_Log("Could not load platform.png texture!");
    }

}

void Platform::render() const {
    texture.render(platformRectangle.x,platformRectangle.y);
}

