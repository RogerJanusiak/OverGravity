#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "../TEng/Texture.h"

class Explosion {

public:

  Explosion(int _x, int _y, SDL_Renderer* renderer);
  ~Explosion();
  void render() const;
  void advance() { frame++; }
  bool finished() { return frame == 10; }

private:
  int x;
  int y;

  int frame = 0;

  Texture texture;

};



#endif //ANIMATIONS_H
