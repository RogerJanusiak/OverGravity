#ifndef PLAYER_H
#define PLAYER_H
#include <list>
#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Spawn.h"

class Player {
public:
    Player(Entity* entity);

    void move(float dt,const std::list<Platform*> &platforms);
    void render() const;
    void changeWeapon();

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    Entity* getEntity() const { return playerEntity; }
    int getWeapon() const { return currentWeapon; }

    void damage() { playerHealth--; }
    int getHP() const { return playerHealth; }
    void setHP(int newHP) { playerHealth = newHP; }

    SDL_Rect playerHealth1;
    SDL_Rect playerHealth2;

private:
    const int playerWidth = 50*SCALE_FACTOR;
    const int playerHeight = 60*SCALE_FACTOR;

    bool playerDirection = false;

    Entity* playerEntity;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    int currentWeapon;
    int playerHealth = 2;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;
};



#endif //PLAYER_H
