#ifndef PLAYER_H
#define PLAYER_H
#include <list>

#include "Bullet.h"
#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Sound.h"
#include "Spawn.h"

enum class Weapon {
    revolver,
    knife,
    lazerPistol,
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

    bool damage();
    int getHP() const { return playerHealth; }
    void setHP(int newHP) { playerHealth = newHP; }

    void decreaseShield();
    int getShield() const { return playerShield; }

    void increaseCombo(int comboToGetShield);
    void zeroCombo() { combo = 0;}
    int getCombo() const { return combo; }

    bool shoot(std::list<Entity>* eBullets, std::list<Bullet>* bullets, int bulletSpeed);
    int reload(float dt);
    bool wasJustReloaded();

    int charge(float dt);
    bool useAbility();

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
    bool justReloaded = false;

    bool charged = true;
    bool justCharged = false;

    const double revolverReloadSpeed = 0.75;
    const double lazerPistolReloadSpeed = 0.5;
    double weaponReloadSpeed = revolverReloadSpeed;

    const double abilityReloadSpeed = 15;

    float timeSinceShot;
    float timeSinceAbilty;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;

    Texture lazerPistolTexture;

    Sound gunshot;
    Sound damageSound;
};

#endif //PLAYER_H
