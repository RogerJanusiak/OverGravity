#ifndef ENEMY_H
#define ENEMY_H
#include <list>
#include <SDL_events.h>
#include <utility>
#include <vector>

#include "Entity.h"
#include "Platform.h"
#include "Texture.h"


class Robor {

public:
    explicit Robor(Entity* entity);

    void move(float dt,const std::list<Platform*> &platforms);
    void render() const { roborEntity->render(); }
    Entity* getEntity() const { return roborEntity; }

    bool alive = true;

private:
    const int enemyWidth = 35;
    const int enemyHeight = 75;

    Entity* roborEntity;
};

#endif //ENEMY_H
