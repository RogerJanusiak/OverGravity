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
    spawns = nullptr;
    gameRender = tempGameRenderer;
}

void Entity::render() const {
    entityTexture.render(entityRect.x,entityRect.y);
}

Platform* Entity::onPlatform(const std::list<Platform*> &platforms, SDL_Rect& movementBox) const {
    for (auto platform : platforms) {
        SDL_Rect p = platform->getPlatformRect();
        if(isColliding(p,movementBox)) {
            return platform;
        }
    }

    return nullptr;
}

bool Entity::move(float dt,const std::list<Platform*> &platforms) {
    Platform* currentPlatform = onPlatform(platforms,entityRect);
    getRect().x += getXVelocity()*dt;
    if(currentPlatform == nullptr) {
        float nextYPosition = entityRect.y + yVelocity*dt;
        SDL_Rect movementBox;
        movementBox.x = entityRect.x;
        movementBox.y = entityRect.y;
        movementBox.w = entityRect.w;
        movementBox.h = entityRect.h + (nextYPosition-entityRect.y);
        SDL_RenderDrawRect(gameRender,&movementBox);
        Platform* potentialPlatform = onPlatform(platforms,movementBox);

        if(potentialPlatform == nullptr) {
            entityRect.y = nextYPosition;
            yVelocity += ACCELERATION*dt;
        } else {
            entityRect.y = potentialPlatform->getPlatformRect().y-entityRect.h;
            yVelocity = 0;
        }
        return true;
    } else {
        entityRect.y = currentPlatform->getPlatformRect().y-entityRect.h;
        yVelocity = 0;
        return false;
    }
}

void Entity::spawn() {
    auto currentSpawnIt = spawns->begin() + (rand() % spawns->size());
    if(!currentSpawnIt->getOccupied()) {
        spawned = true;
        setPosition(currentSpawnIt->getX(),currentSpawnIt->getY());
        setYVelocity(0);
        currentSpawnIt->setOccupied(true);
    }
}


bool Entity::isColliding(SDL_Rect& rectA, const SDL_Rect& rectB) {
    if (rectA.y + rectA.h >= rectB.y && rectA.y <= rectB.y + rectB.h && rectA.x + rectA.w >= rectB.x && rectA.x <= rectB.x + rectB.w) {
        return true;
    }
    return false;
}



