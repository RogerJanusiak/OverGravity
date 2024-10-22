#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "Bullet.h"
#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Sound.h"
#include "Spawn.h"
#include "Weapon.h"

#include <exception>

enum Ability {
    bounce,
    respawn,
    c4,
    none,
};

class Player {
public:
    Player(Entity* entity, Weapon* primaryWeapon);

    int move(float dt,const std::list<Platform*> &platforms, int camY);
    void render() const;

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    bool getDirection() const { return playerDirection; }

    Entity* getEntity() const { return playerEntity; }

    Weapon* getWeapon() { return currentWeapon; }
    void setSecondaryWeapon(Weapon* secWeapon) { secondaryWeapon = secWeapon; currentWeapon = primaryWeapon; }
    Weapon* getSecondaryWeapon() { return secondaryWeapon; }
    void changeWeapon();

    bool damage();
    int getHP() const { return playerHealth; }
    void setHP(int newHP) { playerHealth = newHP; }

    void decreaseShield();
    int getShield() const { return playerShield; }

    void increaseCombo();
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    int charge(float dt);
    Ability useAbility();
    Ability getAbility() { return currentAbility; }
    void setAbility(Ability ability) { currentAbility = ability; c4left = 3; }
    int getC4Left() { return c4left; }

    Entity* getC4Entity() { return &c4Entity; }

    SDL_Rect* getWheelRect() { return &wheelRect; }
    void updateWheelRect() {
        wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
        wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;
    }

    SDL_Rect playerHealth1;
    SDL_Rect playerHealth2;
    SDL_Rect playerShield1;
    SDL_Rect playerShield2;

private:
    const int playerWidth = scale(50);
    const int playerHeight = scale(60);

    bool playerDirection = false;

    Entity* playerEntity;

    SDL_Rect wheelRect;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    Weapon* currentWeapon = nullptr;
    Weapon* primaryWeapon = nullptr;
    Weapon* secondaryWeapon = nullptr;

    Ability currentAbility = Ability::none;

    int playerHealth = 2;
    int playerShield = 0;

    int combo = 0;
    bool topLevelShieldHit = false;

    bool charged = true;
    bool justCharged = false;

    const double abilityReloadSpeed = 15;
    const int comboToGetShield = 15;

    float timeSinceShot;
    float timeSinceAbilty;

    SDL_Rect weaponRect;

    //C4 Ability Variables
    Entity c4Entity;
    Texture c4Texture;
    Texture detinatorTexture;
    bool c4Placed = false;
    int c4left = 3;

    Sound damageSound;
};

#endif //PLAYER_H
