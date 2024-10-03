#include "../includes/Robor.h"

#include <iostream>
#include <list>
#include <vector>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Robor::Robor(Entity *entity) {
    roborEntity = entity;
    roborEntity->setDimensions(enemyWidth,enemyHeight);
    roborEntity->getTexture()->setup(enemyWidth,enemyHeight,entity->getRenderer());
    if(!roborEntity->getTexture()->loadFromFile("resources/robor.png")) {
        SDL_Log("Could not load enemy texture!");
    }
}

void Robor::move(float dt,const std::list<Platform*> &platforms) {
    roborEntity->move(dt,platforms);
    if(roborEntity->getRect().x >= 1200) {
        roborEntity->setPosition(10,roborEntity->getRect().y);
    }

    if(roborEntity->getRect().y >= 683) {
        roborEntity->setPosition(0,46);
    }

    if(roborEntity->getRect().x <= 0) {
        roborEntity->setPosition(1190, roborEntity->getRect().y);
    }

    if(rand() % 500 == 1 && roborEntity->getRect().x <= 1000 && roborEntity->getRect().x >= 200) {
        roborEntity->setXVelocity(roborEntity->getXVelocity()*-1);
    }

    roborEntity->getRect().x += roborEntity->getXVelocity()*dt;
}


