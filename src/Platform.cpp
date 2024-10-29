#include "../includes/Platform.h"

Platform::Platform(int x, int y, SDL_Renderer* renderer) : renderer(renderer) {
    box.x = scale(x);
    box.y = scale(y);
    box.h = height;
    box.w = width;

    texture.setup(width,height,renderer);

    if(!texture.loadFromFile("platform.png")) {
        SDL_Log("Could not load platform.png texture!");
    }

}

void Platform::render(){
    texture.render(box.x,box.y);
    SDL_SetRenderDrawColor( renderer, 20, 20, 20, 80 );
    SDL_RenderFillRect( renderer, &box );
}

void Platform::setPosition(int x, int y) {
    box.x = x;
    box.y = y;
}

