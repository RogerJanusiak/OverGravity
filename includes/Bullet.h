#ifndef BULLET_H
#define BULLET_H
#include <list>

#include "Entity.h"

enum BULLET_TYPE {
    normal,
    laser,
};

class Bullet {

public:

    explicit Bullet(Entity* tempEntity, BULLET_TYPE type);

    void render();
    bool move(float dt, const std::list<Platform*> &platforms, bool developerMode);

    Entity* getEntity() const { return entity; }

    void setIterator(const std::list<Entity>::iterator it) { iterator = it; }
    std::list<Entity>::iterator getIterator() const { return iterator; }

private:

    const int normalWidth = scale(6);
    const int normalHeight = scale(6);

    const int lazerWidth = scale(32);
    const int lazerHeight = scale(4);

    int platformStatus = 0;

    BULLET_TYPE type;

    std::list<Entity>::iterator iterator;

    Entity* entity;

};



#endif //BULLET_H
