#include "../includes/Platform.h"

Platform::Platform(const int x, const int y, SDL_Renderer* renderer) : platformRectangle(scale(x),scale(y),height,width), renderer(renderer) {
    texture.setup(width,height,renderer);

    if(!texture.loadFromFile("platform.png")) {
        SDL_Log("Could not load platform.png texture!");
    }

}

void Platform::render() const {
    texture.render(platformRectangle.x,platformRectangle.y);
}

