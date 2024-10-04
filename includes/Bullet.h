#ifndef BULLET_H
#define BULLET_H
#include <list>

#include "Entity.h"


class Bullet {

public:

    explicit Bullet(Entity* tempEntity);

    void render();
    bool move(float dt) const;

    Entity* getEntity() const { return entity; }

    void setIterator(const std::list<Entity>::iterator it) { iterator = it; }
    std::list<Entity>::iterator getIterator() const { return iterator; }

private:

    const int width = 3*SCALE_FACTOR;
    const int height = 3*SCALE_FACTOR;

    std::list<Entity>::iterator iterator;

    Entity* entity;

};



#endif //BULLET_H
