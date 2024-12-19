#ifndef ENTITY_H
#define ENTITY_H
#include <list>
#include <SDL_rect.h>
#include <vector>

#include "../HEng/Platform.h"
#include "../TEng/Texture.h"
#include "../HEng/Spawn.h"

class Entity {

public:

    Entity() = default;
    Entity(SDL_Renderer* renderer, const int hp) : hp(hp), gameRender(renderer) {}
    Entity(std::vector<Spawn>* spawn, SDL_Renderer* renderer, const int hp) : hp(hp), spawns(spawn), gameRender(renderer) {}
    Entity(int x, int y, int Vx, int Vy,SDL_Renderer *tempGameRenderer);

    void render(int spriteX, int spriteY, bool faceVelocity = false, bool direction = false) const;

    void setSpawns(std::vector<Spawn>* spawn) { spawns = spawn; }

    void setPosition (int x,int y) { entityRect.x = x; entityRect.y = y; }
    void setXVelocity (const float Vx) { xVelocity = Vx; }
    void setYVelocity (const float Vy) { yVelocity = Vy; }
    void setDimensions (const int w, const int h) { entityRect.w = w; entityRect.h = h; }
    void setTexture (const Texture& texture) { entityTexture = texture; }
    void setSource (const int w, const int h) { srcHeight = h; srcWidth = w; }

    void setPhysics(int x, int y, int Vx, int Vy) {  entityRect.x = x; entityRect.y = y; yVelocity = Vy; xVelocity = Vx; }

    SDL_Rect &getRect() { return entityRect; }
    Texture *getTexture() { return &entityTexture; }
    [[nodiscard]] float getXVelocity() const { return xVelocity; }
    [[nodiscard]] float getYVelocity() const { return yVelocity; }
    [[nodiscard]] SDL_Renderer* getRenderer() const { return gameRender; }

    [[nodiscard]] bool isSpawned() const { return spawned; }
    void spawn(bool spawnOnScreen = false);
    void forceSpawn();
    void despawn() { spawned = false;justSpawned = true;entityRect.x = -1000,entityRect.y = -1000; }

    bool move(float dt,const std::list<Platform> &platforms, int* amountFallen = nullptr, SDL_Rect* movementHitBox = nullptr);

    static bool isColliding(SDL_Rect& rectA, const SDL_Rect& rectB);

    bool damage(int damageAmount);
    [[nodiscard]] int getHP() const { return hp; }
    [[nodiscard]] bool isAlive() const { return alive; }

    bool justSpawned = true;

private:
    float xVelocity = 0;
    float yVelocity = 0;

    int srcWidth = 0;
    int srcHeight = 0;

    bool alive = true;
    int hp = 1;

    //TODO: Render the texture once in main.cpp and then pass it to the entity.
    Texture entityTexture;

    Platform* lastPlatform = nullptr;
    bool offPlatform = false;

    std::vector<Spawn>* spawns = nullptr;
    bool spawned = false;

    bool isOnPlatform = true;

    SDL_Rect entityRect = {-150,-150,0,0};
    SDL_Renderer* gameRender;

};

#endif //ENTITY_H
