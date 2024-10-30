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

    void knifeColliding() { knifeCollision = true; }
    void knifeNotColliding() { knifeCollision = false; }
    [[nodiscard]] bool didAlreadyCollide() const { return knifeCollision; }

    static int getDifficulty() { return 1; }

private:
    const int enemyWidth = scale(24);
    const int enemyHeight = scale(50);

    const int xVelocity = scale(250);

    bool knifeCollision = false;

    Entity* roborEntity;
};

#endif //ENEMY_H
