#include "../includes/Weapon.h"
#include "../includes/GlobalConstants.h"
#include "../includes/State.h"

Weapon::Weapon(const Weapon_Type _type, SDL_Renderer* _renderer, State &state) {
  type = _type;
  renderer = _renderer;
  upgrade(state);
  switch (type) {
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

        fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
        reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
        emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      } break;
      case rifle: {
        texture.setup(scale(32*2),scale(12*2),renderer);
        if(!texture.loadFromFile("rifle.png")) {
          SDL_Log("Could not load revolver texture!");
        }

        relXRight = scale(20);
        relXLeft = -scale(30);
        relY = scale(15);

        bulletRelXRight = scale(19);
        bulletRelXLeft = 0;
        bulletRelY = scale(19);
        bulletSpeed = scale(1000);

        //TODO: Add unique sounds
        fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
        reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
        emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      }break;
      case shotgun: {
        texture.setup(scale(28*1.5),scale(9*1.5),renderer);
        if(!texture.loadFromFile("shotgun.png")) {
          SDL_Log("Could not load revolver texture!");
        }

        relXRight = scale(28);
        relXLeft = -scale(26);
        relY = scale(18);

        bulletRelXRight = scale(19);
        bulletRelXLeft = 0;
        bulletRelY = scale(19);
        bulletSpeed = scale(1000);

        //TODO: Add unique sounds
        fireSound.init("resources/sounds/revolver-shoot.wav",0,-1);
        reloadSound.init("resources/sounds/revolver-reload.wav", 0,-1);
        emptySound.init("resources/sounds/revolver-empty.wav", 0,-1);
      }break;
      case knife: {
        texture.setup(scale(42),scale(21),renderer);
        if(!texture.loadFromFile("knife.png")) {
          SDL_Log("Could not load knife texture!");
        }

        relXRight = scale(40);
        relXLeft = -scale(27);
        relY = scale(15);

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

        reloadSpeed = 3;
        timeSinceShot = reloadSpeed;
        fireSound.init("resources/sounds/laserPistol-shoot.wav",0,-1);
        reloadSound.init("resources/sounds/laserPistol-reload.wav", 0,-1);
        emptySound.init("resources/sounds/laserPistol-empty.wav", 0,-1);
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
  if(reloaded && !state.c4Placed && type != knife) {
    fireSound.play();

    if(direction) {
      eBullets->emplace_back(playerX+bulletRelXRight,playerY+bulletRelY,bulletSpeed,0,renderer);
    } else {
      eBullets->emplace_back(playerX+bulletRelXLeft,playerY+bulletRelY,-bulletSpeed,0,renderer);
    }
    bullets->emplace_back(&eBullets->back(), bulletType, bulletDurability, bulletStrength, bulletDamage);
    bullets->back().setIterator(--eBullets->end());

    bulletsInClip--;
    if(bulletsInClip == 0) {
      reloaded = false;
    }
    return true;
  }
  emptySound.play();
  return false;
}

void Weapon::upgrade(const State& state) {
    if(type == revolver) {
        switch(state.currentRevolverLevel) {
            case 1: {
              clipSize = 4;
              reloadSpeed = 3;
              bulletDurability = 1;
              bulletStrength = 1;
              bulletDamage = 1;
            } break;
            case 2: {
              clipSize = 5;
              reloadSpeed = 3;
              bulletDurability = 1;
              bulletStrength = 1;
              bulletDamage = 1;
            } break;
            case 3: {
              clipSize = 6;
              reloadSpeed = 2;
              bulletDurability = 2;
              bulletStrength = 1;
              bulletDamage = 1;
            } break;
            case 4: {
              clipSize = 7;
              reloadSpeed = 2;
              bulletDurability = 2;
              bulletStrength = 1;
              bulletDamage = 2;
            } break;
            case 5: {
              clipSize = 8;
              reloadSpeed = 1;
              bulletDurability = 2;
              bulletStrength = 1;
              bulletDamage = 2;
            } break;
            default:
                break;
        }
    } else if(type == rifle) {
      switch(state.currentRifleLevel) {
        case 1: {
          clipSize = 2;
          reloadSpeed = 3;
          bulletDurability = 1;
          bulletStrength = 1;
          bulletDamage = 1;
        } break;
        case 2: {
          clipSize = 2;
          reloadSpeed = 3;
          bulletDurability = 2;
          bulletStrength = 2;
          bulletDamage = 2;
        } break;
        case 3: {
          clipSize = 3;
          reloadSpeed = 2;
          bulletDurability = 2;
          bulletStrength = 2;
          bulletDamage = 3;
        } break;
        case 4: {
          clipSize = 3;
          reloadSpeed = 2;
          bulletDurability = 2;
          bulletStrength = 3;
          bulletDamage = 4;
        } break;
        case 5: {
          clipSize = 4;
          reloadSpeed = 2;
          bulletDurability = 2;
          bulletStrength = 3;
          bulletDamage = 5;
        } break;
        default:
          break;
      }
    }
}

void Weapon::reset() {
  timeSinceShot = reloadSpeed;
  reloaded = true;
  justReloaded = false;
  bulletsInClip = clipSize;
}