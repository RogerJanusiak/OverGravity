//
// Created by roger on 9/30/2024.
//

#include "../includes/Entity.h"

#include "../includes/Robor.h"

#include <iostream>
#include <list>
#include <vector>

#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"

Entity::Entity(const int x,const int y,const int Vx,const int Vy,SDL_Renderer *tempGameRenderer) {
    entityRect.x = x;
    entityRect.y = y;
    entityRect.w = 0;
    entityRect.h = 0;
    xVelocity = Vx;
    yVelocity = Vy;
    gameRender = tempGameRenderer;
}

void Entity::render() const {
    entityTexture.render(entityRect.x,entityRect.y);
}

Platform* Entity::onPlatform(const std::list<Platform*> &platforms,const int y) const {
    const int enemyLeft = entityRect.x+18;
    const int enemyRight = entityRect.x + entityRect.w-18;
    const int enemyTop = y+60;
    const int enemyBottom = y + entityRect.h;

    for (auto platform : platforms) {
        SDL_Rect p = platform->getPlatformRect();
        const int platformLeft = p.x;
        const int platformRight = p.x + p.w;
        const int platformTop = p.y;
        const int platformBottom = p.y + p.h;

        if (enemyBottom >= platformTop && enemyTop <= platformBottom && enemyRight >= platformLeft && enemyLeft <= platformRight) {
            return platform;
        }
    }

    return nullptr;
}

void Entity::move(float dt,const std::list<Platform*> &platforms) {
    Platform* currentPlatform = onPlatform(platforms,entityRect.y);
    if(currentPlatform == nullptr) {
        float nextYPosition = entityRect.y + yVelocity*dt;
        Platform* potentialPlatform = onPlatform(platforms,nextYPosition);

        if(potentialPlatform == nullptr) {
            entityRect.y = nextYPosition;
            yVelocity += ACCELERATION*dt;
        } else {
            entityRect.y = potentialPlatform->getPlatformRect().y-entityRect.h;
            yVelocity = 0;
        }
    } else {
        entityRect.y = currentPlatform->getPlatformRect().y-entityRect.h;
        yVelocity = 0;
    }

}

bool Entity::isColliding(SDL_Rect& rectA, const SDL_Rect& rectB) {
    if (rectA.y + rectA.h >= rectB.y && rectA.y <= rectB.y + rectB.h && rectA.x + rectA.w >= rectB.x && rectA.x <= rectB.x + rectB.w) {
        return true;
    }
    return false;
}



