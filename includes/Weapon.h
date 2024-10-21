#ifndef WEAPON_H
#define WEAPON_H
#include "Bullet.h"
#include "Sound.h"
#include "State.h"
#include "Texture.h"

#include <list>

enum Weapon_Type {
  revolver,
  knife,
  laserPistol,
  machineGun,
};

class Weapon {

public:

  explicit Weapon(Weapon_Type _type, SDL_Renderer* _renderer);

  void render(int playerX, int playerY, bool playerDirection) const;

  [[nodiscard]] Weapon_Type getType() const { return type; }

  int reload(float dt);
  void forceReload();
  [[nodiscard]] bool isReloadable() const { return reloadable; }
  bool wasJustReloaded();

  void shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets, const State &state, bool direction, int playerX, int playerY);
  [[nodiscard]] int getBulletsInClip() const { return bulletsInClip; }
  [[nodiscard]] int getClipSize() const { return clipSize; }

  void reset();

private:

  SDL_Renderer* renderer;

  Weapon_Type type;
  BULLET_TYPE bulletType = normal;

  int relXRight;
  int relXLeft;
  int relY;

  int bulletRelXRight = 0;
  int bulletRelXLeft = 0;
  int bulletRelY = 0;
  int bulletSpeed = 0;

  int totalClips = 0;
  int clipSize = 0;

  int clipsLeft = 0;
  int bulletsInClip = 0;

  Texture texture;
  Sound fireSound;

  double reloadSpeed = 0;

  bool reloadable = true;
  bool reloaded = true;
  bool justReloaded = false;
  float timeSinceShot = 0;

};

#endif //WEAPON_H
