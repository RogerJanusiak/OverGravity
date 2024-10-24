#ifndef ROBORTO_H
#define ROBORTO_H

#include <list>
#include "Entity.h"
#include "Platform.h"
#include "State.h"

class Roborto {
public:
    explicit Roborto(Entity* entity);

    void move(float dt,const std::list<Platform*> &platforms, State& state);
    void render() const { roborEntity->render(); }
    Entity* getEntity() const { return roborEntity; }

    void pathFind(int x, int y, int& leftWeight, int& rightWeight, State state, bool firstCall);

    bool alive = true;

private:
    const int enemyWidth = scale(24);
    const int enemyHeight = scale(50);

    const int xVelocity = scale(250);

    Entity* roborEntity;
};



#endif //ROBORTO_H
