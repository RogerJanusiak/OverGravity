#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <SDL_rect.h>
#include <vector>

#include "Platform.h"
#include "Spawn.h"
#include "Texture.h"


class Entity {

public:

    Entity() = default;
    Entity(std::vector<Spawn>* spawn, SDL_Renderer* renderer) : spawns(spawn), gameRender(renderer) {}
    Entity(int x, int y, int Vx, int Vy,SDL_Renderer *tempGameRenderer);

    void render() const;

    void setPosition (int x,int y) { entityRect.x = x; entityRect.y = y; }
    void setXVelocity (const float Vx) { xVelocity = Vx; }
    void setYVelocity (const float Vy) { yVelocity = Vy; }
    void setDimensions (const int w, const int h) { entityRect.w = w; entityRect.h = h; }
    void setTexture (const Texture& texture) { entityTexture = texture; }

    void setPhysics(int x, int y, int Vx, int Vy) {  entityRect.x = x; entityRect.y = y; yVelocity = Vy; xVelocity = Vx; }

    SDL_Rect &getRect() { return entityRect; }
    Texture *getTexture() { return &entityTexture; }
    float getXVelocity() const { return xVelocity; }
    float getYVelocity() const { return yVelocity; }
    SDL_Renderer* getRenderer() const { return gameRender; }

    bool isSpawned() const { return spawned; }
    void spawn();
    void forceSpawn();
    void despawn() { spawned = false;justSpawned = true;entityRect.x = -1000,entityRect.y = -1000; }

    bool move(float dt,const std::list<Platform*> &platforms, int* amountFallen = nullptr, SDL_Rect* movementHitBox = nullptr);

    static bool isColliding(SDL_Rect& rectA, const SDL_Rect& rectB);

    bool justSpawned = true;

private:
    float xVelocity = 0;
    float yVelocity = 0;

    //TODO: Render the texture once in main.cpp and then pass it to the entity.
    Texture entityTexture;

    Platform* onPlatform(const std::list<Platform*> &platforms, SDL_Rect& movementBox, SDL_Rect& hitBox) const;

    Platform* lastPlatform = nullptr;
    bool offPlatform = false;

    std::vector<Spawn>* spawns;
    bool spawned = false;

    bool isOnPlatform = true;

    SDL_Rect entityRect;
    SDL_Renderer* gameRender;

};

#endif //ENTITY_H
