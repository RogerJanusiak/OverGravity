#include "../includes/Player.h"

#include <iostream>
#include <list>
#include <vector>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity, Spawn _spawn) : playerEntity(entity), spawn(_spawn) {

    currentWeapon = 1;

    weaponRect.x = 0;
    weaponRect.y = 0;
    weaponRect.w = 64;
    weaponRect.h = 16;

    playerEntity->setDimensions(playerWidth,playerHeight);
    playerTextureLeft.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    playerTextureRight.setup(playerWidth,playerHeight,playerEntity->getRenderer());
    knifeTextureLeft.setup(64,32,playerEntity->getRenderer());
    knifeTextureRight.setup(64,32,playerEntity->getRenderer());
    revolverTextureLeft.setup(64,32,playerEntity->getRenderer());
    revolverTextureRight.setup(64,32,playerEntity->getRenderer());

    if(!playerTextureRight.loadFromFile("resources/Timpy.png")) {
        SDL_Log("Could not load TimpyRight texture!");
    }
    if(!playerTextureLeft.loadFromFile("resources/TimpyLeft.png")) {
        SDL_Log("Could not load TimpyLeft texture!");
    }
    if(!knifeTextureLeft.loadFromFile("resources/knife.png")) {
        SDL_Log("Could not load knife texture!");
    }
    if(!knifeTextureRight.loadFromFile("resources/knife-right.png")) {
        SDL_Log("Could not load knife texture!");
    }
    if(!revolverTextureLeft.loadFromFile("resources/RevolverLeft.png")) {
        SDL_Log("Could not load RevolverLeft texture!");
    }
    if(!revolverTextureRight.loadFromFile("resources/Revolver.png")) {
        SDL_Log("Could not load Revolver texture!");
    }

    playerEntity->setTexture(playerTextureLeft);

}

void Player::render() const {
    playerEntity->render();

    if(currentWeapon == 1) {
        if(playerDirection) {
            revolverTextureRight.render(playerEntity->getRect().x+60,playerEntity->getRect().y+22);
        } else {
            revolverTextureLeft.render(playerEntity->getRect().x-40,playerEntity->getRect().y+22);
        }
    } else {
        if(playerDirection) {
            knifeTextureRight.render(playerEntity->getRect().x+60,playerEntity->getRect().y+22);
        } else {
            knifeTextureLeft.render(playerEntity->getRect().x-40,playerEntity->getRect().y+22);
        }
    }



}

void Player::move(float dt,const std::list<Platform*> &platforms) {
    playerEntity->move(dt,platforms);

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+60;
    } else {
        weaponRect.x = playerEntity->getRect().x-40;
    }

    weaponRect.y = playerEntity->getRect().y+30;

    if(playerEntity->getRect().x >= 1200) {
        playerEntity->setPosition(-64,playerEntity->getRect().y);
    }

    if(playerEntity->getRect().y >= 683) {
        playerEntity->setYVelocity(0);
        playerEntity->setXVelocity(0);
        playerEntity->setPosition(spawn.getX(),spawn.getY());
    }

    if(playerEntity->getRect().x < -64) {
        playerEntity->setPosition(1200, playerEntity->getRect().y);
    }

    playerEntity->getRect().x += playerEntity->getXVelocity()*dt;

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
    if(currentWeapon == 2) {
        currentWeapon = 1;
    } else {
        currentWeapon = 2;
    }


}
