#ifndef PLAYER_H
#define PLAYER_H
#include <list>
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

    void move(float dt,const std::list<Platform*> &platforms);
    void render() const;
    void changeWeapon();

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    bool getDirection () const { return playerDirection; }
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

    void shoot() const;

    SDL_Rect playerHealth1;
    SDL_Rect playerHealth2;
    SDL_Rect playerShield1;
    SDL_Rect playerShield2;

private:
    const int playerWidth = scale(50);
    const int playerHeight = scale(60);

    bool playerDirection = false;

    Entity* playerEntity;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    Weapon currentWeapon;

    int playerHealth = 2;
    int playerShield = 0;

    int combo = 0;
    bool topLevelShieldHit = false;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;
};



#endif //PLAYER_H
