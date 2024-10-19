#ifndef ENEMY_H
#define ENEMY_H
#include <list>
#include "Entity.h"
#include "Platform.h"


class Robor {

public:
    explicit Robor(Entity* entity);

    void move(float dt,const std::list<Platform*> &platformsm, int levelY, int levelHeight) const;
    void render() const { roborEntity->render(); }
    Entity* getEntity() const { return roborEntity; }

    bool alive = true;

private:
    const int enemyWidth = scale(24);
    const int enemyHeight = scale(50);

    const int xVelocity = scale(250);

    Entity* roborEntity;
};

#endif //ENEMY_H
