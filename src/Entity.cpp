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

Platform* Entity::onPlatform(const std::list<Platform*> &platforms, SDL_Rect& movementBox, SDL_Rect& hitBox) const {
    for (auto platform : platforms) {
        SDL_Rect p = platform->getPlatformRect();
        if(hitBox.y+hitBox.h <= platform->getPlatformRect().y) {
            if(isColliding(p,movementBox)) {
                return platform;
            }
        }

    }

    return nullptr;
}

bool Entity::move(float dt,const std::list<Platform*> &platforms, int* amountFallen, SDL_Rect* movementHitBox) {
    int startY = entityRect.y;
    getRect().x += getXVelocity()*dt;
    float nextYPosition = entityRect.y + yVelocity*dt;
    offPlatform = true;

    SDL_Rect movementBox;

    if(movementHitBox == nullptr) {
        movementBox.x = entityRect.x;
        movementBox.w = entityRect.w;
    } else {
        movementBox.x = movementHitBox->x;
        movementBox.w = movementHitBox->w;
    }

    movementBox.y = entityRect.y;
    movementBox.h = entityRect.h + (nextYPosition-entityRect.y);
    Platform* potentialPlatform = onPlatform(platforms,movementBox, entityRect);

    if(potentialPlatform == nullptr) {
        entityRect.y = nextYPosition;
        yVelocity += ACCELERATION*dt;
    } else {
        offPlatform = false;
        entityRect.y = potentialPlatform->getPlatformRect().y-entityRect.h;
        yVelocity = 0;
    }
    if(amountFallen != nullptr) {
        *amountFallen = startY - entityRect.y;
    }
    return offPlatform;
}

void Entity::spawn() {
    auto currentSpawnIt = spawns->begin() + (rand() % spawns->size());
    if(!currentSpawnIt->getOccupied()) {
        offPlatform = false;
        spawned = true;
        setPosition(currentSpawnIt->getX(),currentSpawnIt->getY());
        setYVelocity(0);
        currentSpawnIt->setOccupied(true);
    }
}

void Entity::forceSpawn() {
    for(auto it = spawns->begin(); it != spawns->end(); it++) {
        offPlatform = false;
        spawned = true;
        setPosition(it->getX(),it->getY());
        setYVelocity(0);
        it->setOccupied(true);
        break;
    }
}


bool Entity::isColliding(SDL_Rect& rectA, const SDL_Rect& rectB) {
    if (rectA.y + rectA.h >= rectB.y && rectA.y <= rectB.y + rectB.h && rectA.x + rectA.w >= rectB.x && rectA.x <= rectB.x + rectB.w) {
        return true;
    }
    return false;
}



