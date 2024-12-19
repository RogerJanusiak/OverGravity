#include "../../includes/Entities/Bullet.h"

Bullet::Bullet(Entity* tempEntity, BULLET_TYPE type, int durability, int strength, int damage) : entity(tempEntity), type(type), durability(durability), strength(strength), damage(damage) {
    switch(type) {
        case BULLET_TYPE::laser: {
            entity->setDimensions(lazerWidth,lazerHeight);
            entity->getTexture()->setup(lazerWidth,lazerHeight,entity->getRenderer());
            entity->setSource(32,8);
            if(!entity->getTexture()->loadFromFile("laser.png")) {
                SDL_Log("Could not load bullet texture!");
            }
        } break;
        default: {
            entity->setDimensions(normalWidth,normalHeight);
            entity->getTexture()->setup(normalWidth,normalHeight,entity->getRenderer());
            entity->setSource(4,4);
            if(!entity->getTexture()->loadFromFile("bullet.png")) {
                SDL_Log("Could not load bullet texture!");
            }
        }
    }


}

bool Bullet::move(float dt, const std::list<Platform> &platforms, bool developerMode) {
    trailingRect.x = getEntity()->getRect().x;
    trailingRect.y = getEntity()->getRect().y;
    if(type == BULLET_TYPE::normal) {
        if(!entity->move(dt, platforms)) {
            if(platformStatus == 0) {
                platformStatus = 1;
            } else if(platformStatus == 2) {
                if(durability == 1) {
                    return true;
                }
                platformStatus = 1;
            }
        } else if(platformStatus == 1) {
            platformStatus = 2;
        }
    } else if(type == BULLET_TYPE::laser) {
        entity->setPosition(entity->getRect().x + entity->getXVelocity()*dt,entity->getRect().y);
    }
    trailingRect.w = abs(trailingRect.x - getEntity()->getRect().x) + getEntity()->getRect().w;
    trailingRect.h = abs(trailingRect.y - getEntity()->getRect().y) + getEntity()->getRect().h;

    if(entity->getRect().x > WINDOW_WIDTH || entity->getRect().x < 0) {
        return true;
    }
    return false;
}

void Bullet::render() const {
    entity->render(0,0);
}

