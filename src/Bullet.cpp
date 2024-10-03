//
// Created by roger on 10/1/2024.
//

#include "../includes/Bullet.h"

Bullet::Bullet(Entity* tempEntity) : entity(tempEntity) {
    entity->setDimensions(width,height);
    entity->getTexture()->setup(width,height,entity->getRenderer());
    if(!entity->getTexture()->loadFromFile("resources/bullet.png")) {
        SDL_Log("Could not load bullet texture!");
    }
}

bool Bullet::move(float dt) const {
    entity->setPosition(entity->getRect().x + entity->getXVelocity()*dt,entity->getRect().y);
    if(entity->getRect().x > 1200 || entity->getRect().x < 0) {
        return true;
    }
    return false;
}

void Bullet::render() {
    entity->render();
}

