#ifndef ENEMY_H
#define ENEMY_H
#include <list>

#include "Entity.h"
#include "Platform.h"


class Robor {

public:
    explicit Robor(Entity* entity, int Vx);

    void move(float dt,const std::list<Platform*> &platformsm, int levelX) const;
    void render() const { roborEntity->render(); }
    Entity* getEntity() const { return roborEntity; }

    bool alive = true;

private:
    const int enemyWidth = scale(24);
    const int enemyHeight = scale(50);

    int xVelocity;

    Entity* roborEntity;
};

#endif //ENEMY_H
