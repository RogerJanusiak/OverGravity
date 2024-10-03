#ifndef PLAYER_H
#define PLAYER_H
#include <list>
#include "Platform.h"
#include "Texture.h"

#include "Entity.h"
#include "Spawn.h"

class Player {
public:
    Player(Entity* entity, Spawn _spawn);

    void move(float dt,const std::list<Platform*> &platforms);
    void render() const;
    void changeWeapon();

    const SDL_Rect &getWeaponRect() const { return weaponRect; }

    void setDirection (bool direction);
    Entity* getEntity() const { return playerEntity; }
    int getWeapon() const { return currentWeapon; }

    Spawn& getSpawn() { return spawn; }

private:
    const int playerWidth = 80;
    const int playerHeight = 80;

    Spawn spawn;

    bool playerDirection = false;

    Entity* playerEntity;

    Texture playerTextureRight;
    Texture playerTextureLeft;

    int currentWeapon;

    SDL_Rect weaponRect;
    Texture knifeTextureRight;
    Texture knifeTextureLeft;

    Texture revolverTextureRight;
    Texture revolverTextureLeft;
};



#endif //PLAYER_H
