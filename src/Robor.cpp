#include "../includes/Robor.h"

#include <iostream>
#include <list>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Robor::Robor(Entity *entity) {
    roborEntity = entity;
    roborEntity->setDimensions(enemyWidth,enemyHeight);
    roborEntity->getTexture()->setup(enemyWidth,enemyHeight,entity->getRenderer());
    if(!roborEntity->getTexture()->loadFromFile("robor.png")) {
        SDL_Log("Could not load enemy texture!");
    }
}

void Robor::move(const float dt,const std::list<Platform*> &platforms) const {
    if(roborEntity->justSpawned) {
        roborEntity->justSpawned = false;
        if(roborEntity->getRect().x <=400*SCALE_FACTOR) {
            roborEntity->setXVelocity(200*SCALE_FACTOR);
        } else {
            roborEntity->setXVelocity(-200*SCALE_FACTOR);
        }
    }
    roborEntity->move(dt,platforms);
    if(roborEntity->getRect().x >= WINDOW_WIDTH) {
        roborEntity->setPosition(10,roborEntity->getRect().y);
    }

    if(roborEntity->getRect().y >= WINDOW_HEIGHT) {
        roborEntity->despawn();
    }

    if(roborEntity->getRect().x <= 0) {
        roborEntity->setPosition(WINDOW_WIDTH-10*SCALE_FACTOR, roborEntity->getRect().y);
    }

}


