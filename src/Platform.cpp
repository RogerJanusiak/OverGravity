//
// Created by Roger Janusiak on 9/27/24.
//

#include "../includes/Platform.h"

Platform::Platform(int x, int y, SDL_Renderer* renderer) {
    box.x = x*1.5;
    box.y = (y-25)*1.5;
    box.h = height;
    box.w = width;

    texture.setup(width,height,renderer);

    if(!texture.loadFromFile("resources/platform.png")) {
        SDL_Log("Could not load platform.png texture!");
    }

}

void Platform::render() {
    texture.render(box.x,box.y);
}
