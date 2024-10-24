#include "../includes/Player.h"
#include "../includes/Entity.h"
#include "../includes/Bullet.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity, Weapon* primaryWeapon) : playerEntity(entity), primaryWeapon(primaryWeapon) {

    currentWeapon = primaryWeapon;

    damageSound.init("resources/sounds/playerDamage.wav",0,-1);

    wheelRect.w = scale(13);
    wheelRect.h = scale(13);

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

    c4Texture.setup(scale(32),scale(32),playerEntity->getRenderer());
    detinatorTexture.setup(scale(32),scale(32),playerEntity->getRenderer());

    if(!playerTextureRight.loadFromFile("Timpy.png")) {
        SDL_Log("Could not load TimpyRight texture!");
    }
    if(!playerTextureLeft.loadFromFile("TimpyLeft.png")) {
        SDL_Log("Could not load TimpyLeft texture!");
    }

    if(!c4Texture.loadFromFile("C4.png")) {
        SDL_Log("Could not load c4 texture!");
    }
    if(!detinatorTexture.loadFromFile("Detinator.png")) {
        SDL_Log("Could not load detinator texture!");
    }

    c4Entity.setTexture(c4Texture);
    c4Entity.setDimensions(scale(32),scale(32));
    playerEntity->setTexture(playerTextureLeft);

}

void Player::render() const {
    if(c4Placed) {
        c4Entity.render();
    }

    playerEntity->render();

    if(!c4Placed) {
        currentWeapon->render(playerEntity->getRect().x,playerEntity->getRect().y,playerDirection);
    } else {
        if(playerDirection) {
            detinatorTexture.render(playerEntity->getRect().x+scale(35),playerEntity->getRect().y+scale(12));
        } else {
            detinatorTexture.render(playerEntity->getRect().x-scale(17),playerEntity->getRect().y+scale(12),SDL_FLIP_HORIZONTAL);
        }
    }
}

int Player::move(float dt,const std::list<Platform*> &platforms,int camY) {

    if(c4Placed) {
        c4Entity.move(dt,platforms);
    }

    int amountFallen = 0;
    playerEntity->move(dt,platforms,&amountFallen,&wheelRect);

    if(playerDirection) {
        weaponRect.x = playerEntity->getRect().x+scale(40);
    } else {
        weaponRect.x = playerEntity->getRect().x-scale(27);
    }
    weaponRect.y = playerEntity->getRect().y+scale(20);

    if(playerEntity->getRect().x >= WINDOW_WIDTH) {
        playerEntity->setPosition(-scale(40),playerEntity->getRect().y);
    } else if(playerEntity->getRect().x < -scale(40)) {
        playerEntity->setPosition(WINDOW_WIDTH, playerEntity->getRect().y);
    }

    if(playerEntity->getRect().y > WINDOW_HEIGHT) {
        playerEntity->forceSpawn();
    }

    wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
    wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;

    return amountFallen;
}

void Player::setDirection(bool direction) {
    playerDirection = direction;
    if (direction == true) {
        playerEntity->setTexture(playerTextureRight);
    } else {
        playerEntity->setTexture(playerTextureLeft);
    }
}

void Player::increaseCombo() {
    combo++;
    if(combo == comboToGetShield && (playerShield == 0 || topLevelShieldHit)) {
        playerShield++;
        topLevelShieldHit = false;
    } else if(combo == comboToGetShield*2 && playerShield == 1) {
        playerShield++;
    }
}

int Player::charge(float dt) {
    if (timeSinceAbilty >= abilityReloadSpeed) {
        timeSinceAbilty = 0;
        charged = true;
        justCharged = true;
        return 75;
    }
    if(!charged) {
        timeSinceAbilty += dt;
        return 75*timeSinceAbilty*(1/abilityReloadSpeed)-2;
    }
    return 75;
}

Ability Player::useAbility() {

    if (charged && currentAbility == respawn) {
        charged = false;
        return respawn;
    }
    if (currentAbility == c4 && !c4Placed && c4left > 0) {
        c4Entity.setPhysics(playerEntity->getRect().x,playerEntity->getRect().y+playerEntity->getRect().h - scale(32),0,0);
        c4Placed = true;
        c4left--;
        return none;
    }
    if(currentAbility == c4 && c4Placed) {
        c4Placed = false;
        return c4;
    }
  return none;
}

bool Player::damage() {
    if(playerShield == 2) {
        playerShield--;
        topLevelShieldHit = true;
    } else if(playerShield == 1) {
        playerShield--;
    } else {
        if(playerHealth == 2) {
            playerHealth--;
        } else if (playerHealth == 1) {
            playerHealth = 2;
            combo = 0;
            return true;
        }
    }
    damageSound.play();
    return false;
}

void Player::changeWeapon() {

    if(currentWeapon == primaryWeapon && secondaryWeapon != nullptr) {
        currentWeapon = secondaryWeapon;
    } else if(currentWeapon == secondaryWeapon) {
        currentWeapon = primaryWeapon;
    }

}