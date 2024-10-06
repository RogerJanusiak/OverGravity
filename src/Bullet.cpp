#include "../includes/Bullet.h"

Bullet::Bullet(Entity* tempEntity) : entity(tempEntity) {
    entity->setDimensions(width,height);
    entity->getTexture()->setup(width,height,entity->getRenderer());
    if(!entity->getTexture()->loadFromFile("bullet.png")) {
        SDL_Log("Could not load bullet texture!");
    }
}

bool Bullet::move(float dt, const std::list<Platform*> &platforms, bool developerMode) const {
    if(developerMode) {
        entity->move(dt, platforms);
    } else {
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

