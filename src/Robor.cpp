#include "../includes/Robor.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Robor::Robor(Entity *entity, const int Vx) {
    xVelocity = Vx;
    roborEntity = entity;
    roborEntity->setDimensions(enemyWidth,enemyHeight);
    roborEntity->getTexture()->setup(enemyWidth,enemyHeight,entity->getRenderer());
    if(!roborEntity->getTexture()->loadFromFile("robor.png")) {
        SDL_Log("Could not load enemy texture!");
    }
}

void Robor::move(const float dt,const std::list<Platform*> &platforms, int levelX) const {
    if(roborEntity->justSpawned) {
        roborEntity->justSpawned = false;
        if(roborEntity->getRect().x <=scale(400)) {
            roborEntity->setXVelocity(xVelocity);
        } else {
            roborEntity->setXVelocity(-xVelocity);
        }
    }
    roborEntity->move(dt,platforms);
    if((roborEntity->getRect().x+roborEntity->getRect().w) >= LEVEL_WIDTH+levelX || roborEntity->getRect().x < levelX) {
        roborEntity->setXVelocity(-1*roborEntity->getXVelocity());
    }

    if(roborEntity->getRect().y >= WINDOW_HEIGHT) {
        roborEntity->despawn();
    }
}


