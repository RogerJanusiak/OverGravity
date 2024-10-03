#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <SDL_rect.h>
#include <vector>

#include "Platform.h"
#include "Texture.h"


class Entity {

public:

    Entity() = default;
    Entity(int x, int y, int Vx, int Vy,SDL_Renderer *tempGameRenderer);

    void render() const;

    void setPosition (int x,int y) { entityRect.x = x; entityRect.y = y; yVelocity = 0; }
    void setXVelocity (const float Vx) { xVelocity = Vx; }
    void setYVelocity (const float Vy) { yVelocity = Vy; }
    void setDimensions (const int w, const int h) { entityRect.w = w; entityRect.h = h; }
    void setTexture (const Texture& texture) { entityTexture = texture; }

    void initialize(int x, int y, int Vx, int Vy,SDL_Renderer *tempGameRenderer) {  entityRect.x = x; entityRect.y = y; yVelocity = Vy; xVelocity = Vx; gameRender = tempGameRenderer; }

    SDL_Rect &getRect() { return entityRect; }
    Texture *getTexture() { return &entityTexture; }
    float getXVelocity() const { return xVelocity; }
    float getYVelocity() const { return yVelocity; }
    SDL_Renderer* getRenderer() const { return gameRender; }

    bool isSpawned() const { return spawned; }
    void setSpawned(bool _spawned) { spawned = _spawned; }

    void move(float dt,const std::list<Platform*> &platforms);

    static bool isColliding(SDL_Rect& rectA, const SDL_Rect& rectB);

    Platform* onPlatform(const std::list<Platform*> &platforms, int y) const;

private:
    float xVelocity;
    float yVelocity;
    //TO-DO: Render the texture once in main.cpp and then pass it to the entity.
    Texture entityTexture;

    bool spawned = false;

    SDL_Rect entityRect;
    SDL_Renderer* gameRender;

};

#endif //ENTITY_H
