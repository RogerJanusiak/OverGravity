#include "../../includes/Utils/Animations.h"
#include "../../includes/Utils/GlobalConstants.h"

Explosion::Explosion(const int _x, const int _y, SDL_Renderer* renderer) {
    x = _x-scale(16);
    y = _y-scale(16);
    texture.setup(scale(32),scale(32),renderer);
    if(!texture.loadFromFile("explosion.png")) {
      SDL_Log("Could not load explosion texture!");
    }
 }

 Explosion::~Explosion() {
  texture.free();
}

void Explosion::render() const {
  int scaledExplosionFrame = frame/2;
  SDL_Rect sprite = {27*scaledExplosionFrame,0,27,27};
  texture.render(x,y,SDL_FLIP_NONE,&sprite);
}

