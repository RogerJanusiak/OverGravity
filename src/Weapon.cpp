#include "../includes/Weapon.h"
#include "../includes/GlobalConstants.h"
#include "../includes/State.h"

Weapon::Weapon(const Weapon_Type _type, SDL_Renderer* _renderer) {
  type = _type;
  renderer = _renderer;
  switch (type) {
    case knife: {
      texture.setup(scale(42),scale(21),renderer);
      if(!texture.loadFromFile("knife.png")) {
        SDL_Log("Could not load knife texture!");
      }

      relXRight = scale(40);
      relXLeft = -scale(27);
      relY = scale(15);

      totalBullets = 0;
      totalBulletsLeft = 0;

      reloadable = false;
    } break;
    case laserPistol: {
      texture.setup(scale(42),scale(21),renderer);
      if(!texture.loadFromFile("laserPistol.png")) {
        SDL_Log("Could not load laserPistol texture!");
      }

      relXRight = scale(30);
      relXLeft = -scale(22);
      relY = scale(12);

      bulletRelXRight = scale(30);
      bulletRelXLeft = 0;
      bulletRelY = scale(17);
      bulletSpeed = scale(1200);
      bulletType = laser;

      clipSize = 10;
      totalBullets = 50;
      totalBulletsLeft = totalBullets;

      reloadSpeed = 3;
      timeSinceShot = reloadSpeed;
      fireSound.init("resources/sounds/laserPistol-shoot.wav",0,-1);
      reloadSound.init("resources/sounds/laserPistol-reload.wav", 0,-1);
      emptySound.init("resources/sounds/laserPistol-empty.wav", 0,-1);
    } break;
  default: {
    texture.setup(scale(42),scale(21),renderer);
    if(!texture.loadFromFile("Revolver.png")) {
      SDL_Log("Could not load revolver texture!");
    }

    relXRight = scale(40);
    relXLeft = -scale(27);
    relY = scale(15);

    bulletRelXRight = scale(19);
    bulletRelXLeft = 0;
    bulletRelY = scale(19);
    bulletSpeed = scale(1000);

    clipSize = 6;

    reloadSpeed = 2;
    timeSinceShot = reloadSpeed;
    fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
    reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
    emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
  } break;
  }

}

void Weapon::render(const int playerX, const int playerY, const bool playerDirection) const {

  if(playerDirection) {
    texture.render(playerX+relXRight,playerY+relY);
  } else {
    texture.render(playerX+relXLeft,playerY+relY, SDL_FLIP_HORIZONTAL);
  }

}

int Weapon::reload(float dt) {
  if(totalBulletsLeft == 0 && type != revolver) {
    return 0;
  }
  if (timeSinceShot >= reloadSpeed) {
      timeSinceShot = 0;
      reloaded = true;
      bulletsInClip = clipSize;
      justReloaded = true;
      return 75;
  }
  if(!reloaded) {
      timeSinceShot += dt;
      return 75*timeSinceShot*(1/reloadSpeed)-2;
  }
  return 75;
}

void Weapon::forceReload() {

  reloaded = false;
  bulletsInClip = 0;

}

bool Weapon::wasJustReloaded() {
  if(justReloaded) {
    justReloaded = false;
    reloadSound.play();
    return true;
  }
  return false;
}

bool Weapon::shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets, const State &state, bool direction, int playerX, int playerY) {
  if(reloaded && !state.c4Placed && type != knife && (totalBulletsLeft > 0 || type == revolver)) {
    fireSound.play();

    if(direction) {
      eBullets->emplace_back(playerX+bulletRelXRight,playerY+bulletRelY,bulletSpeed,0,renderer);
    } else {
      eBullets->emplace_back(playerX+bulletRelXLeft,playerY+bulletRelY,-bulletSpeed,0,renderer);
    }
    bullets->emplace_back(&eBullets->back(), bulletType);
    bullets->back().setIterator(--eBullets->end());

    bulletsInClip--;
    if(type != revolver) {
      totalBulletsLeft--;
    }
    if(bulletsInClip == 0) {
      reloaded = false;
    }
    return true;
  }
  emptySound.play();
  return false;
}

void Weapon::reset() {
  timeSinceShot = reloadSpeed;
  reloaded = true;
  justReloaded = false;
  totalBulletsLeft = totalBullets;
  bulletsInClip = clipSize;
}