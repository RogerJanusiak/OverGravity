#include "../../includes/HEng/Weapon.h"
#include "../../includes/Utils/GlobalConstants.h"
#include "../../includes/HEng/State.h"

Weapon::Weapon(GlobalGameState& ggs, const Weapon_Type type, SDL_Renderer* renderer) : ggs(ggs), renderer(renderer), type(type) {
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
  upgrade();
  reset();
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

bool Weapon::shoot(std::list<Entity>& eBullets, std::list<Bullet>& bullets, bool direction, int playerX, int playerY) {
  if(reloaded) {
    fireSound.play();

    for(int i = 0; i < bulletsPerShot; i++) {
      if(direction) {
        eBullets.emplace_back(playerX+bulletRelXRight,playerY+bulletRelY,bulletSpeed+(i)*75,-150*i,renderer);
      } else {
        eBullets.emplace_back(playerX+bulletRelXLeft,playerY+bulletRelY,-bulletSpeed+(i)*75,-150*i,renderer);
      }
      bullets.emplace_back(&eBullets.back(), bulletType, bulletDurability, bulletStrength, bulletDamage);
      bullets.back().setIterator(--eBullets.end());
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
      if(bulletsInClip <= 0) {
        reloaded = false;
      }
    }


    timeSinceShot = 0;
    return true;
  }
  emptySound.play();
  return false;
}

void Weapon::upgrade() {
    if(type == revolver) {
        int level = ggs.weaponLevels[revolver] == 0 ? 0 : ggs.weaponLevels[revolver] - 1;
        clipSize = ggs.weaponProperties[revolver][level][1];
        reloadSpeed = ggs.weaponProperties[revolver][level][2];
        bulletDurability = ggs.weaponProperties[revolver][level][3];
        bulletStrength = ggs.weaponProperties[revolver][level][4];
        bulletDamage = ggs.weaponProperties[revolver][level][5];
    } else if(type == rifle) {
      int level = ggs.weaponLevels[rifle] == 0 ? 0 : ggs.weaponLevels[rifle] - 1;
      clipSize = ggs.weaponProperties[rifle][level][1];
      reloadSpeed = ggs.weaponProperties[rifle][level][2];
      bulletDurability = ggs.weaponProperties[rifle][level][3];
      bulletStrength = ggs.weaponProperties[rifle][level][4];
      bulletDamage = ggs.weaponProperties[rifle][level][5];
    } else if(type == shotgun) {
      int level = ggs.weaponLevels[shotgun] == 0 ? 0 : ggs.weaponLevels[shotgun] - 1;
      clipSize = ggs.weaponProperties[shotgun][level][1];
      reloadSpeed = ggs.weaponProperties[shotgun][level][2];
      bulletDurability = ggs.weaponProperties[shotgun][level][3];
      bulletDamage = ggs.weaponProperties[shotgun][level][4];
      bulletsPerShot = ggs.weaponProperties[shotgun][level][5];
    } else if(type == laserPistol) {
      int level = ggs.weaponLevels[laserPistol] == 0 ? 0 : ggs.weaponLevels[laserPistol] - 1;
      clipSize = ggs.weaponProperties[laserPistol][level][2];
      reloadSpeed = ggs.weaponProperties[laserPistol][level][2];
      coolFireRate = ggs.weaponProperties[laserPistol][level][1];
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