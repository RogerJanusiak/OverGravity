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

void Robor::move(const float dt,const std::list<Platform*> &platforms, int levelY, int levelHeight) const {
    if(roborEntity->justSpawned) {
        roborEntity->justSpawned = false;
        if(roborEntity->getRect().x <=scale(400)) {
            roborEntity->setXVelocity(xVelocity);
        } else {
            roborEntity->setXVelocity(-xVelocity);
        }
    }
    roborEntity->move(dt,platforms);
    if(roborEntity->getRect().y >= scale(levelHeight)+levelY) {
        roborEntity->despawn();
    }

    if(roborEntity->getRect().x >= WINDOW_WIDTH) {
        roborEntity->setPosition(scale(10),roborEntity->getRect().y);
    } else if(roborEntity->getRect().x <= 0) {
        roborEntity->setPosition(WINDOW_WIDTH-scale(10), roborEntity->getRect().y);
    }


}


