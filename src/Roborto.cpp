#include "../includes/Roborto.h"

Roborto::Roborto(Entity *entity) {
    roborEntity = entity;
    roborEntity->setDimensions(enemyWidth,enemyHeight);
    roborEntity->getTexture()->setup(enemyWidth,enemyHeight,entity->getRenderer());
    if(!roborEntity->getTexture()->loadFromFile("roborto.png")) {
        SDL_Log("Could not load enemy texture!");
    }
}

int findEdgeRight(int startX, int startY, State state) {
    bool lookingRight = true;
    int numberTilesRight = 1;
    while(lookingRight) {
        int rightProbe = startX+numberTilesRight;
        if(rightProbe > 11) {
            numberTilesRight = -1;
            lookingRight = false;
        } else {
            if(state.levelMap[startY][rightProbe] != -1) {
                numberTilesRight++;
            } else {
                lookingRight = false;
            }
        }
    }
    return numberTilesRight;
}

int findEdgeLeft(int startX, int startY, State state) {
    bool lookingLeft = true;
    int numberTilesLeft = 1;
    while(lookingLeft) {
        int leftProbe = startX-numberTilesLeft;
        if(leftProbe < 0) {
            numberTilesLeft = -1;
            lookingLeft = false;
        } else {
            if(state.levelMap[startY][leftProbe] != -1) {
                numberTilesLeft++;
            } else {
                lookingLeft = false;
            }
        }
    }
    return numberTilesLeft;
}

void Roborto::pathFind(int x, int y, int& leftWeight, int& rightWeight, State state) {

    if(y < state.playerTileY) {
        // Find right weight
        int rightEdge = findEdgeRight(x,y,state);
        if(rightEdge < 0){
            rightWeight = 1000;
        } else {
            int openTileRight = x + rightEdge;
            bool fallingRight = true;
            int numberTilesDownRight = 1;
            while(fallingRight) {
                if(state.levelMap[y+numberTilesDownRight][openTileRight] == -1) {
                    numberTilesDownRight++;
                } else {
                    fallingRight = false;
                }
            }

            int tempRRightWeight;
            int tempRLeftWeight;

            pathFind(openTileRight,y+numberTilesDownRight, tempRRightWeight, tempRLeftWeight, state);

            if(tempRLeftWeight >= tempRRightWeight) {
                rightWeight = tempRRightWeight + rightEdge;
            } else {
                rightWeight = tempRLeftWeight + rightEdge;
            }
        }


        // Find left weight
        int leftEdge = findEdgeLeft(x,y,state);
        if(leftEdge < 0){
            leftWeight = 1000;
        } else {
            int openTileLeft = x - leftEdge;
            bool fallingLeft = true;
            int numberTilesDownLeft = 1;
            while(fallingLeft) {
                if(state.levelMap[y+numberTilesDownLeft][openTileLeft] == -1) {
                    numberTilesDownLeft++;
                } else {
                    fallingLeft = false;
                }
            }

            int tempLRightWeight;
            int tempLLeftWeight;

            pathFind(openTileLeft,y+numberTilesDownLeft, tempLLeftWeight, tempLRightWeight, state);

            if(tempLLeftWeight >= tempLRightWeight) {
                leftWeight = tempLRightWeight + leftEdge;
            } else {
                leftWeight = tempLLeftWeight + leftEdge;
            }
        }
    } else if(y == state.playerTileY) {

        int xDifference = state.playerTileX - x;
        int direction = 1;
        if(xDifference != 0) {
            direction = xDifference/abs(xDifference);
        }

        bool allPlatforms = true;
        for(int i = 1; i < abs(state.playerTileX - x); i++) {
            if(state.levelMap[y][x+(i*direction)] == -1) {
                allPlatforms = false;
            }
        }

        if (allPlatforms) {
            if (direction > 0) {
                rightWeight = 0;
                leftWeight = 1000;
            } else {
                rightWeight = 1000;
                leftWeight = 0;
            }
        } else {
            rightWeight = 1000;
            leftWeight = 1000;
        }
    } else {
        rightWeight = 1000;
        leftWeight = 1000;
    }
}

void Roborto::move(float dt,const std::list<Platform*> &platforms, State& state) {
    roborEntity->move(dt,platforms);

    //Path Finding
    int tileX = roborEntity->getRect().x/TILE_SIZE_SCALED+1;
    int tileY = (roborEntity->getRect().y/TILE_SIZE_SCALED);
    int xDifference = state.playerX - roborEntity->getRect().x;
    int direction = 1;
    if(xDifference != 0) {
        direction = xDifference/abs(xDifference);
    }
    if(tileY < state.levelMap.size() && state.levelMap[tileY][tileX] != -1) {
        int leftWeight;
        int rightWeight;
        pathFind(tileX,tileY,leftWeight,rightWeight,state);
        if(leftWeight >= 1000 && rightWeight >= 1000) {
            int numberTilesRight = findEdgeRight(tileX,tileY,state);
            int numberTilesLeft = findEdgeLeft(tileX,tileY,state);

            numberTilesRight = numberTilesRight > 0 ? numberTilesRight : 1000;
            numberTilesLeft = numberTilesLeft > 0 ? numberTilesLeft : 1000;

            if(numberTilesLeft > numberTilesRight) {
                roborEntity->setXVelocity(xVelocity);
            } else if(numberTilesLeft < numberTilesRight) {
                roborEntity->setXVelocity(xVelocity*-1);
            } else if(roborEntity->getXVelocity() == 0) {
                roborEntity->setXVelocity(xVelocity);
            }
        } else {
            if(leftWeight > rightWeight) {
                roborEntity->setXVelocity(xVelocity);
            } else if(leftWeight < rightWeight) {
                roborEntity->setXVelocity(-xVelocity);
            } else if(roborEntity->getXVelocity() == 0) {
                roborEntity->setXVelocity(xVelocity);
            }
        }
    }

    if(roborEntity->getRect().y >= scale(state.levelHeight)+state.camY) {
        roborEntity->despawn();
    }

    //TODO: Move to entity
    if(roborEntity->getRect().x >= WINDOW_WIDTH) {
        roborEntity->setPosition(scale(10),roborEntity->getRect().y);
    } else if(roborEntity->getRect().x <= 0) {
        roborEntity->setPosition(WINDOW_WIDTH-scale(10), roborEntity->getRect().y);
    }


}