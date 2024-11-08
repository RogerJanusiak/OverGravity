#include "../includes/Roborto.h"

void Roborto::pathFind(int x, int y, int& leftWeight, int& rightWeight, State state, bool firstCall = false) {
    if(y < state.playerTileY) {
        // Find right weight
        int rightEdge = findEdgeRight(x,y,state);
        int openTileRight = x + rightEdge;
        if(rightEdge < 0){
            rightWeight = 1000;
        } else {
            bool fallingRight = true;
            int numberTilesDownRight = 1;
            while(fallingRight) {
                if(y+numberTilesDownRight < state.levelMap.size() && state.levelMap[y+numberTilesDownRight][openTileRight] == -1) {
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
                if(y+numberTilesDownLeft < state.levelMap.size() && state.levelMap[y+numberTilesDownLeft][openTileLeft] == -1) {
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
        int xDifference = firstCall ? state.playerX - entity->getRect().x : state.playerTileX-x;
        int direction = 1;
        if(xDifference != 0) {
            direction = xDifference/abs(xDifference);
        }

        bool allPlatforms = true;
        if(state.playerTileX != x-1 || state.playerTileX != x+1) {
            for(int i = 1; i < abs(state.playerTileX - x); i++) {
                if(y < state.levelMap.size() && state.levelMap[y][x+(i*direction)] == -1) {
                    allPlatforms = false;
                }
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
    entity->move(dt,platforms);

    //Path Finding
    int tileX = entity->getRect().x/TILE_SIZE_SCALED+1;
    int tileY = (entity->getRect().y-state.camY)/TILE_SIZE_SCALED;
    if(tileY < state.levelMap.size() && state.levelMap[tileY][tileX] != -1) {
        int leftWeight;
        int rightWeight;
        pathFind(tileX,tileY,leftWeight,rightWeight,state, true);
        if(leftWeight >= 1000 && rightWeight >= 1000) {
            int numberTilesRight = findEdgeRight(tileX,tileY,state);
            int numberTilesLeft = findEdgeLeft(tileX,tileY,state);

            numberTilesRight = numberTilesRight > 0 ? numberTilesRight : 1000;
            numberTilesLeft = numberTilesLeft > 0 ? numberTilesLeft : 1000;

            if(numberTilesLeft > numberTilesRight) {
                entity->setXVelocity(xVelocity);
            } else if(numberTilesLeft < numberTilesRight) {
                entity->setXVelocity(xVelocity*-1);
            } else if(entity->getXVelocity() == 0) {
                entity->setXVelocity(xVelocity);
            }
        } else {
            if(leftWeight > rightWeight) {
                entity->setXVelocity(xVelocity);
            } else if(leftWeight < rightWeight) {
                entity->setXVelocity(-xVelocity);
            } else if(entity->getXVelocity() == 0) {
                entity->setXVelocity(xVelocity);
            }
        }
    }

    if(entity->getRect().y >= scale(state.levelHeight)+state.camY) {
        entity->despawn();
    }

    //TODO: Move to entity
    if(entity->getRect().x >= WINDOW_WIDTH) {
        entity->setPosition(scale(10),entity->getRect().y);
    } else if(entity->getRect().x <= 0) {
        entity->setPosition(WINDOW_WIDTH-scale(10), entity->getRect().y);
    }


}