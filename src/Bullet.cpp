#include "../includes/Bullet.h"

Bullet::Bullet(Entity* tempEntity, BULLET_TYPE type) : entity(tempEntity), type(type) {
    switch(type) {
        case BULLET_TYPE::laser: {
            entity->setDimensions(lazerWidth,lazerHeight);
            entity->getTexture()->setup(lazerWidth,lazerHeight,entity->getRenderer());
            if(!entity->getTexture()->loadFromFile("laser.png")) {
                SDL_Log("Could not load bullet texture!");
            }
        } break;
        default: {
            entity->setDimensions(normalWidth,normalHeight);
            entity->getTexture()->setup(normalWidth,normalHeight,entity->getRenderer());
            if(!entity->getTexture()->loadFromFile("bullet.png")) {
                SDL_Log("Could not load bullet texture!");
            }
        }
    }


}

bool Bullet::move(float dt, const std::list<Platform*> &platforms, bool developerMode) {
    if(type == BULLET_TYPE::normal) {
        if(!entity->move(dt, platforms)) {
            if(platformStatus == 0) {
                platformStatus = 1;
            } else if(platformStatus == 2) {
                return true;
            }
        } else if(platformStatus == 1) {
            platformStatus = 2;
        }
    } else if(type == BULLET_TYPE::laser) {
        entity->setPosition(entity->getRect().x + entity->getXVelocity()*dt,entity->getRect().y);
    }

    if(entity->getRect().x > WINDOW_WIDTH || entity->getRect().x < 0) {
        return true;
    }
    return false;
}

void Bullet::render() {
    entity->render();
}

