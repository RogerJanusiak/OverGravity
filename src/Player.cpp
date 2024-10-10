#include "../includes/Player.h"
#include "../includes/Entity.h"
#include "../includes/Bullet.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity) : playerEntity(entity) {

    currentWeapon = Weapon::revolver;

    weaponRect.x = 0;
    weaponRect.y = 0;
    weaponRect.w = scale(42);
    weaponRect.h = scale(10);

    playerHealth1.x = scale(15);
    playerHealth1.y = WINDOW_HEIGHT-scale(45);
    playerHealth1.w = scale(30);
    playerHealth1.h = scale(30);

    playerHealth2.x = scale(15);
    playerHealth2.y = WINDOW_HEIGHT-scale(85);
    playerHealth2.w = scale(30);
    playerHealth2.h = scale(30);

    playerShield1.x = scale(15);
    playerShield1.y = WINDOW_HEIGHT-scale(125);
    playerShield1.w = scale(30);
    playerShield1.h = scale(30);

    playerShield2.x = scale(15);
    playerShield2.y = WINDOW_HEIGHT-scale(165);
    playerShield2.w = scale(30);
    playerShield2.h = scale(30);

    playerEntity->setDimensions(playerWidth,playerHeight);
    playerTextureLeft.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    playerTextureRight.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    knifeTextureLeft.setup(scale(42),scale(21),playerEntity->getRenderer());
    knifeTextureRight.setup(scale(42),scale(21),playerEntity->getRenderer());
    revolverTextureLeft.setup(scale(42),scale(21),playerEntity->getRenderer());
    revolverTextureRight.setup(scale(42),scale(21),playerEntity->getRenderer());

    if(!playerTextureRight.loadFromFile("Timpy.png")) {
        SDL_Log("Could not load TimpyRight texture!");
    }
    if(!playerTextureLeft.loadFromFile("TimpyLeft.png")) {
        SDL_Log("Could not load TimpyLeft texture!");
    }
    if(!knifeTextureLeft.loadFromFile("knife.png")) {
        SDL_Log("Could not load knife texture!");
    }
    if(!knifeTextureRight.loadFromFile("knife-right.png")) {
        SDL_Log("Could not load knife texture!");
    }
    if(!revolverTextureLeft.loadFromFile("RevolverLeft.png")) {
        SDL_Log("Could not load RevolverLeft texture!");
    }
    if(!revolverTextureRight.loadFromFile("Revolver.png")) {
        SDL_Log("Could not load Revolver texture!");
    }

    playerEntity->setTexture(playerTextureLeft);

}

void Player::render() const {
    playerEntity->render();

    if(currentWeapon == Weapon::revolver) {
        if(playerDirection) {
            revolverTextureRight.render(playerEntity->getRect().x+scale(40),playerEntity->getRect().y+scale(15));
        } else {
            revolverTextureLeft.render(playerEntity->getRect().x-scale(27),playerEntity->getRect().y+scale(15));
        }
    } else {
        if(playerDirection) {
            knifeTextureRight.render(playerEntity->getRect().x+scale(40),playerEntity->getRect().y+scale(15));
        } else {
            knifeTextureLeft.render(playerEntity->getRect().x-scale(27),playerEntity->getRect().y+scale(15));
        }
    }
}

void Player::move(float dt,const std::list<Platform*> &platforms) {
   playerEntity->move(dt,platforms);

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+scale(40);
    } else {
        weaponRect.x = playerEntity->getRect().x-scale(27);
    }

    weaponRect.y = playerEntity->getRect().y+scale(20);

    if(playerEntity->getRect().x >= WINDOW_WIDTH) {
        playerEntity->setPosition(-scale(40),playerEntity->getRect().y);
    }

    if(playerEntity->getRect().y >= WINDOW_HEIGHT) {
        playerEntity->setYVelocity(0);
        playerEntity->setXVelocity(0);
        playerEntity->spawn();
    }

    if(playerEntity->getRect().x < -scale(40)) {
        playerEntity->setPosition(WINDOW_WIDTH, playerEntity->getRect().y);
    }

}

void Player::setDirection(bool direction) {
    playerDirection = direction;
    if (direction == true) {
        playerEntity->setTexture(playerTextureRight);
    } else {
        playerEntity->setTexture(playerTextureLeft);
    }
}

void Player::changeWeapon() {
    if(currentWeapon == Weapon::knife) {
        currentWeapon = Weapon::revolver;
    } else {
        currentWeapon = Weapon::knife;
    }
}

void Player::increaseCombo(int comboToGetShield) {
    combo++;
    if(combo == comboToGetShield && (getShield() == 0 || topLevelShieldHit)) {
        playerShield++;
        topLevelShieldHit = false;
    } else if(combo == comboToGetShield*2 && getShield() == 1) {
        playerShield++;
    }
}

void Player::decreaseShield() {
    if(playerShield == 2) {
        topLevelShieldHit = true;
    }
    playerShield--;
}

bool Player::shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets, int bulletSpeed) {
    if(reloaded && currentWeapon == Weapon::revolver) {
        if(playerDirection) {
            eBullets->emplace_back(getEntity()->getRect().x+scale(60),getEntity()->getRect().y+scale(19),bulletSpeed,0,getEntity()->getRenderer());
            bullets->emplace_back(&eBullets->back());
            bullets->back().setIterator(--eBullets->end());
        } else {
            eBullets->emplace_back(getEntity()->getRect().x,getEntity()->getRect().y+scale(19),-bulletSpeed,0,getEntity()->getRenderer());
            bullets->emplace_back(&eBullets->back());
            bullets->back().setIterator(--eBullets->end());
        }
        reloaded = false;
        return true;
    }
    return false;
}

int Player::reload(float dt,double revolverReloadSpeed) {
    if (timeSinceShot >= revolverReloadSpeed) {
        timeSinceShot = 0;
        reloaded = true;
        return 75;
    }
    if(!reloaded) {
        timeSinceShot += dt;
        return 75*timeSinceShot*(1/revolverReloadSpeed)-2;
    }
    return 75;
}


