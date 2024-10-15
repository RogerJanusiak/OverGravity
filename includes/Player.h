#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "Bullet.h"
#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Spawn.h"

enum class Weapon {
    revolver,
    knife,
    machineGun,
    c4
};

class Player {
public:
    Player(Entity* entity);

    int move(float dt,const std::list<Platform*> &platforms, int camY);
    void render() const;
    void changeWeapon();

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);

    Entity* getEntity() const { return playerEntity; }
    Weapon getWeapon() const { return currentWeapon; }

    void damage() { playerHealth--; }
    int getHP() const { return playerHealth; }
    void setHP(int newHP) { playerHealth = newHP; }

    void decreaseShield();
    int getShield() const { return playerShield; }

    void increaseCombo(int comboToGetShield);
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    bool shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets, int bulletSpeed);
    int reload(float dt,double revolverReloadSpeed);

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

    Weapon currentWeapon;

    int playerHealth = 2;
    int playerShield = 0;

    int combo = 0;
    bool topLevelShieldHit = false;

    bool reloaded = true;
    float timeSinceShot;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;
};

#endif //PLAYER_H
