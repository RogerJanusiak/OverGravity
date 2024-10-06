#include "../includes/Player.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity) : playerEntity(entity) {

    currentWeapon = Weapon::revolver;

    weaponRect.x = 0;
    weaponRect.y = 0;
    weaponRect.w = 42*SCALE_FACTOR;
    weaponRect.h = 10*SCALE_FACTOR;

    playerHealth1.x = 15*SCALE_FACTOR;
    playerHealth1.y = WINDOW_HEIGHT-45*SCALE_FACTOR;
    playerHealth1.w = 30*SCALE_FACTOR;
    playerHealth1.h = 30*SCALE_FACTOR;

    playerHealth2.x = 15*SCALE_FACTOR;
    playerHealth2.y = WINDOW_HEIGHT-85*SCALE_FACTOR;
    playerHealth2.w = 30*SCALE_FACTOR;
    playerHealth2.h = 30*SCALE_FACTOR;

    playerShield1.x = 15*SCALE_FACTOR;
    playerShield1.y = WINDOW_HEIGHT-125*SCALE_FACTOR;
    playerShield1.w = 30*SCALE_FACTOR;
    playerShield1.h = 30*SCALE_FACTOR;

    playerShield2.x = 15*SCALE_FACTOR;
    playerShield2.y = WINDOW_HEIGHT-165*SCALE_FACTOR;
    playerShield2.w = 30*SCALE_FACTOR;
    playerShield2.h = 30*SCALE_FACTOR;

    playerEntity->setDimensions(playerWidth,playerHeight);
    playerTextureLeft.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    playerTextureRight.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    knifeTextureLeft.setup(42*SCALE_FACTOR,21*SCALE_FACTOR,playerEntity->getRenderer());
    knifeTextureRight.setup(42*SCALE_FACTOR,21*SCALE_FACTOR,playerEntity->getRenderer());
    revolverTextureLeft.setup(42*SCALE_FACTOR,21*SCALE_FACTOR,playerEntity->getRenderer());
    revolverTextureRight.setup(42*SCALE_FACTOR,21*SCALE_FACTOR,playerEntity->getRenderer());

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
            revolverTextureRight.render(playerEntity->getRect().x+40*SCALE_FACTOR,playerEntity->getRect().y+15*SCALE_FACTOR);
        } else {
            revolverTextureLeft.render(playerEntity->getRect().x-27*SCALE_FACTOR,playerEntity->getRect().y+15*SCALE_FACTOR);
        }
    } else {
        if(playerDirection) {
            knifeTextureRight.render(playerEntity->getRect().x+40*SCALE_FACTOR,playerEntity->getRect().y+15*SCALE_FACTOR);
        } else {
            knifeTextureLeft.render(playerEntity->getRect().x-27*SCALE_FACTOR,playerEntity->getRect().y+15*SCALE_FACTOR);
        }
    }
}

void Player::move(float dt,const std::list<Platform*> &platforms) {
    playerEntity->move(dt,platforms);

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+40*SCALE_FACTOR;
    } else {
        weaponRect.x = playerEntity->getRect().x-27*SCALE_FACTOR;
    }

    weaponRect.y = playerEntity->getRect().y+20*SCALE_FACTOR;

    if(playerEntity->getRect().x >= WINDOW_WIDTH) {
        playerEntity->setPosition(-40*SCALE_FACTOR,playerEntity->getRect().y);
    }

    if(playerEntity->getRect().y >= WINDOW_HEIGHT) {
        playerEntity->setYVelocity(0);
        playerEntity->setXVelocity(0);
        playerEntity->spawn();
    }

    if(playerEntity->getRect().x < -40*SCALE_FACTOR) {
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
