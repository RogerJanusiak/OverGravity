#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Sound.h"
#include "Spawn.h"
#include "Weapon.h"

#include <exception>

enum Ability {
    bounce,
    teleport,
    c4,
    grenade,
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
    void setPrimaryWeapon(Weapon* priWeapon) { primaryWeapon = priWeapon; }
    void setSecondaryWeapon(Weapon* secWeapon) { secondaryWeapon = secWeapon; currentWeapon = primaryWeapon; }
    Weapon* getSecondaryWeapon() { return secondaryWeapon; }
    Weapon* getPrimaryWeapon() { return primaryWeapon; }
    void changeWeapon();

    bool damage();
    int getHP() const { return playerHealth; }
    void setHP(int newHP) { playerHealth = newHP; }

    void decreaseShield();
    int getShield() const { return playerShield; }
    void setShield(int newShield) { playerShield = newShield; }

    void increaseCombo();
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    void changeXP(int _xp) { xp += _xp; }
    void setXP(int _xp ) { xp = _xp; }
    int getXP() const { return xp; }

    int charge(float dt, State& state);
    void useAbility(State& state);
    [[nodiscard]] bool isCharged() const { return charged;}
    Ability getAbility() { return currentAbility; }
    void setAbility(Ability ability) { currentAbility = ability; }

    [[nodiscard]] bool isInvincible() const { return invincible; }
    void setInvincible(bool _invincible) { invincible = _invincible; }

    Entity* getC4Entity() { return &c4Entity; }

    SDL_Rect getHitRect() { return playerHitRect; }
    SDL_Rect* getWheelRect() { return &wheelRect; }
    void updateWheelRect() {
        wheelRect.x = getEntity()->getRect().x+((getEntity()->getRect().w-20)/2);
        wheelRect.y = getEntity()->getRect().y+getEntity()->getRect().h-20;
    }

    SDL_Rect playerHealth1;
    SDL_Rect playerHealth2;
    SDL_Rect playerHealth3;
    SDL_Rect playerShield1;
    SDL_Rect playerShield2;

private:
    const int playerWidth = scale(50);
    const int playerHeight = scale(60);

    bool playerDirection = false;

    Entity* playerEntity;

    SDL_Rect playerHitRect;
    SDL_Rect wheelRect;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    Weapon* currentWeapon = nullptr;
    Weapon* primaryWeapon = nullptr;
    Weapon* secondaryWeapon = nullptr;

    Ability currentAbility = Ability::none;

    int playerHealth = 3;
    int playerShield = 0;

    int xp = 0;

    int combo = 0;
    bool topLevelShieldHit = false;

    bool charged = true;
    bool justCharged = false;

    bool invincible = false;

    const int comboToGetShield = 10;

    float timeSinceShot;
    float timeSinceAbilty;

    SDL_Rect weaponRect;

    //C4 Ability Variables
    Entity c4Entity;
    Texture c4Texture;
    Texture detinatorTexture;
    bool c4Placed = false;

    Sound damageSound;
};

#endif //PLAYER_H
