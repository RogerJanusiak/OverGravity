#include "../includes/Player.h"
#include "../includes/Entity.h"
#include "../includes/Bullet.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Player::Player(Entity* entity) : playerEntity(entity) {

    currentWeapon = Weapon::revolver;

    gunshot.init("resources/sounds/gunshot.wav",0,-1);
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
    knifeTextureLeft.setup(scale(42),scale(21),playerEntity->getRenderer());
    knifeTextureRight.setup(scale(42),scale(21),playerEntity->getRenderer());
    revolverTextureLeft.setup(scale(42),scale(21),playerEntity->getRenderer());
    revolverTextureRight.setup(scale(42),scale(21),playerEntity->getRenderer());
    lazerPistolTexture.setup(scale(42),scale(21),playerEntity->getRenderer());

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
    if(!lazerPistolTexture.loadFromFile("lazerPistol.png")) {
        SDL_Log("Could not load lazerPistol texture!");
    }

    playerEntity->setTexture(playerTextureLeft);

}

// TODO: Change all weapons to use flip, maybe timpy too.
void Player::render() const {
    playerEntity->render();

    switch (currentWeapon) {
        case Weapon::lazerPistol: {
            if(playerDirection) {
                lazerPistolTexture.render(playerEntity->getRect().x+scale(30),playerEntity->getRect().y+scale(12));
            } else {
                lazerPistolTexture.render(playerEntity->getRect().x-scale(22),playerEntity->getRect().y+scale(12),SDL_FLIP_HORIZONTAL);
            }
        } break;
        case Weapon::knife: {
            if(playerDirection) {
                knifeTextureRight.render(playerEntity->getRect().x+scale(40),playerEntity->getRect().y+scale(15));
            } else {
                knifeTextureLeft.render(playerEntity->getRect().x-scale(27),playerEntity->getRect().y+scale(15));
            }
        } break;
        default: {
            if(playerDirection) {
                revolverTextureRight.render(playerEntity->getRect().x+scale(40),playerEntity->getRect().y+scale(15));
            } else {
                revolverTextureLeft.render(playerEntity->getRect().x-scale(27),playerEntity->getRect().y+scale(15));
            }
        }
    }
}

int Player::move(float dt,const std::list<Platform*> &platforms,int camY) {

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

void Player::changeWeapon() {
    if(currentWeapon == Weapon::knife) {
        currentWeapon = Weapon::lazerPistol;
        weaponReloadSpeed  = lazerPistolReloadSpeed;
    } else if(currentWeapon == Weapon::lazerPistol) {
        currentWeapon = Weapon::revolver;
        weaponReloadSpeed  = revolverReloadSpeed;
    } else {
        currentWeapon = Weapon::knife;
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

bool Player::shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets) {
    if(reloaded) {
        switch(currentWeapon) {
            case Weapon::lazerPistol: {
                if(playerDirection) {
                    eBullets->emplace_back(getEntity()->getRect().x+scale(30),getEntity()->getRect().y+scale(17),Bullet::getSpeed(),0,getEntity()->getRenderer());
                    bullets->emplace_back(&eBullets->back(), BULLET_TYPE::lazer);
                    bullets->back().setIterator(--eBullets->end());
                } else {
                    eBullets->emplace_back(getEntity()->getRect().x,getEntity()->getRect().y+scale(17),-Bullet::getSpeed(),0,getEntity()->getRenderer());
                    bullets->emplace_back(&eBullets->back(), BULLET_TYPE::lazer);
                    bullets->back().setIterator(--eBullets->end());
                }
                reloaded = false;
                return true;
            }
            default: {
                if(playerDirection) {
                    eBullets->emplace_back(getEntity()->getRect().x+scale(60),getEntity()->getRect().y+scale(19),Bullet::getSpeed(),0,getEntity()->getRenderer());
                    bullets->emplace_back(&eBullets->back(), BULLET_TYPE::normal);
                    bullets->back().setIterator(--eBullets->end());
                } else {
                    eBullets->emplace_back(getEntity()->getRect().x,getEntity()->getRect().y+scale(19),-Bullet::getSpeed(),0,getEntity()->getRenderer());
                    bullets->emplace_back(&eBullets->back(), BULLET_TYPE::normal);
                    bullets->back().setIterator(--eBullets->end());
                }
                gunshot.play();
                reloaded = false;
                return true;
            }
        }
    }
    return false;
}

int Player::reload(float dt) {
    if (timeSinceShot >= weaponReloadSpeed) {
        timeSinceShot = 0;
        reloaded = true;
        justReloaded = true;
        return 75;
    }
    if(!reloaded) {
        timeSinceShot += dt;
        return 75*timeSinceShot*(1/weaponReloadSpeed)-2;
    }
    return 75;
}

bool Player::wasJustReloaded() {
    if(justReloaded) {
        justReloaded = false;
        return true;
    }
    return false;
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

bool Player::useAbility() {
    if(charged) {
        charged = false;
        return true;
    }
    return false;
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

