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
  if(!reloaded) {
      if (timeSinceShot >= reloadSpeed) {
        timeSinceShot = 0;
        reloaded = true;
        if(type == laserPistol) {
          bulletsInClip = 0;
        } else {
          bulletsInClip = clipSize;
        }
        justReloaded = true;
        return 75;
      }
      timeSinceShot += dt;
      return 75*timeSinceShot*(1/reloadSpeed)-2;
  }
  timeSinceShot += dt;
  return 75;
}

void Weapon::forceReload() {
  timeSinceShot = 0;
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

    for(int i = 0; i < bulletsPerShot; i++) {
      if(direction) {
        eBullets->emplace_back(playerX+bulletRelXRight,playerY+bulletRelY,bulletSpeed+(i)*75,-150*i,renderer);
      } else {
        eBullets->emplace_back(playerX+bulletRelXLeft,playerY+bulletRelY,-bulletSpeed+(i)*75,-150*i,renderer);
      }
      bullets->emplace_back(&eBullets->back(), bulletType, bulletDurability, bulletStrength, bulletDamage);
      bullets->back().setIterator(--eBullets->end());
    }

    if(type == laserPistol) {
      if(timeSinceShot <= coolFireRate) {
        bulletsInClip++;
        if(bulletsInClip >= clipSize) {
          reloaded = false;
        }
      }
    } else {
      bulletsInClip--;
      if(bulletsInClip == 0) {
        reloaded = false;
      }
    }


    timeSinceShot = 0;
    return true;
  }
  emptySound.play();
  return false;
}

void Weapon::upgrade(const State& state) {
    if(type == revolver) {
        int level = state.weaponLevels[revolver] == 0 ? 0 : state.weaponLevels[revolver] - 1;
        clipSize = state.weaponProperties[0][level][1];
        reloadSpeed = state.weaponProperties[0][level][2];
        bulletDurability = state.weaponProperties[0][level][3];
        bulletStrength = state.weaponProperties[0][level][4];
        bulletDamage = state.weaponProperties[0][level][5];
    } else if(type == rifle) {
      int level = state.weaponLevels[rifle] == 0 ? 0 : state.weaponLevels[rifle] - 1;
      clipSize = state.weaponProperties[1][level][1];
      reloadSpeed = state.weaponProperties[1][level][2];
      bulletDurability = state.weaponProperties[1][level][3];
      bulletStrength = state.weaponProperties[1][level][4];
      bulletDamage = state.weaponProperties[1][level][5];
    } else if(type == shotgun) {
      int level = state.weaponLevels[shotgun] == 0 ? 0 : state.weaponLevels[shotgun] - 1;
      clipSize = state.weaponProperties[2][level][1];
      reloadSpeed = state.weaponProperties[2][level][2];
      bulletDurability = state.weaponProperties[2][level][3];
      bulletDamage = state.weaponProperties[2][level][4];
      bulletsPerShot = state.weaponProperties[2][level][5];
    } else if(type == knife) {
      int level = state.weaponLevels[knife] == 0 ? 0 : state.weaponLevels[knife] - 1;
      bulletDamage = state.weaponProperties[3][level][1];
    } else if(type == laserPistol) {
      int level = state.weaponLevels[laserPistol] == 0 ? 0 : state.weaponLevels[laserPistol] - 1;
      clipSize = state.weaponProperties[4][level][1];
      reloadSpeed = state.weaponProperties[4][level][2];
      coolFireRate = state.weaponProperties[4][level][3];
      bulletsInClip = 0;
    }
}

void Weapon::reset() {
  timeSinceShot = 0;
  reloaded = true;
  justReloaded = false;
  if(type == laserPistol) {
    bulletsInClip = 0;
  } else {
    bulletsInClip = clipSize;
  }
}