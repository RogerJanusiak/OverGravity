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

    void increaseShield() { playerShield++; }
    void decreaseShield() { playerShield--; }
    int getShield() const { return playerShield; }

    void shoot() const;

    SDL_Rect playerHealth1;
    SDL_Rect playerHealth2;
    SDL_Rect playerShield1;
    SDL_Rect playerShield2;

private:
    const int playerWidth = 50*SCALE_FACTOR;
    const int playerHeight = 60*SCALE_FACTOR;

    bool playerDirection = false;

    Entity* playerEntity;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    Weapon currentWeapon;

    int playerHealth = 2;
    int playerShield = 0;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;
};



#endif //PLAYER_H
