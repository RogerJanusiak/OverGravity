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
            int numberTilesDownRight = 0;
            int newX = openTileRight*TILE_SIZE_SCALED;
            int newXTile;
            float v = 0;
            while(fallingRight) {
                newXTile = newX/TILE_SIZE_SCALED;
                if(y+numberTilesDownRight < state.levelMap.size() && state.levelMap[y+numberTilesDownRight][newXTile] == -1) {
                    numberTilesDownRight++;
                    float dt = (-v+sqrt(2*ACCELERATION*TILE_SIZE_SCALED+pow(v,2)))/ACCELERATION;
                    if(v <= TERMINAL_VELOCITY) {
                        v += ACCELERATION*dt;
                    }
                    newX += getXVelocity()*dt;
                } else {
                    fallingRight = false;
                }
            }

            int tempRRightWeight;
            int tempRLeftWeight;

            SDL_Log("Next Location Right: %i,%i",newXTile,y+numberTilesDownRight);
            pathFind(newXTile,y+numberTilesDownRight, tempRRightWeight, tempRLeftWeight, state);

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
            int numberTilesDownLeft = 0;
            int newX = (openTileLeft+1)*TILE_SIZE_SCALED - getEntity()->getRect().w;
            int newXTile;
            float v = 0;
            while(fallingLeft) {
                newXTile = newX/TILE_SIZE_SCALED;
                if(y+numberTilesDownLeft < state.levelMap.size() && state.levelMap[y+numberTilesDownLeft][newXTile] == -1) {
                    numberTilesDownLeft++;
                    float dt = (-v+sqrt(2*ACCELERATION*TILE_SIZE_SCALED+pow(v,2)))/ACCELERATION;
                    if(v <= TERMINAL_VELOCITY) {
                        v += ACCELERATION*dt;
                    }
                    newX -= getXVelocity()*dt;
                } else {
                    fallingLeft = false;
                }
            }

            int tempLRightWeight;
            int tempLLeftWeight;

            SDL_Log("Next Location Left: %i,%i",newXTile,y+numberTilesDownLeft);
            pathFind(newXTile,y+numberTilesDownLeft, tempLLeftWeight, tempLRightWeight, state);

            if(tempLLeftWeight >= tempLRightWeight) {
                leftWeight = tempLRightWeight + leftEdge;
            } else {
                leftWeight = tempLLeftWeight + leftEdge;
            }
        }
    } else if(y == state.playerTileY) {
        int xTileDifference = state.playerTileX-x;
        int xDifference = firstCall ? state.playerX - entity->getRect().x : xTileDifference;
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
                rightWeight = abs(xTileDifference);
                leftWeight = 1000;
            } else {
                rightWeight = 1000;
                leftWeight = abs(xTileDifference);
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
    if(!entity->move(dt,platforms)) {
        if(!justHitPlatform) {
            //Path Finding
            int tileX = entity->getRect().x/TILE_SIZE_SCALED;
            int tileY = entity->getRect().y/TILE_SIZE_SCALED;
            SDL_Log("Robor Location: %i,%i",tileX,tileY);
            if(tileY < state.levelMap.size() && state.levelMap[tileY][tileX] != -1) {
                int leftWeight;
                int rightWeight;
                pathFind(tileX,tileY,leftWeight,rightWeight,state, true);
                SDL_Log("Weights: %i,%i",leftWeight,rightWeight);
                if(leftWeight >= 1000 && rightWeight >= 1000) {
                    int numberTilesRight = findEdgeRight(tileX,tileY,state);
                    int numberTilesLeft = findEdgeLeft(tileX,tileY,state);

                    numberTilesRight = numberTilesRight > 0 ? numberTilesRight : 1000;
                    numberTilesLeft = numberTilesLeft > 0 ? numberTilesLeft : 1000;

                    if(numberTilesLeft > numberTilesRight) {
                        entity->setXVelocity(getXVelocity());
                    } else if(numberTilesLeft < numberTilesRight) {
                        entity->setXVelocity(getXVelocity()*-1);
                    } else if(entity->getXVelocity() == 0) {
                        entity->setXVelocity(getXVelocity());
                    }
                } else {
                    if(leftWeight > rightWeight) {
                        entity->setXVelocity(getXVelocity());
                    } else if(leftWeight < rightWeight) {
                        entity->setXVelocity(-getXVelocity());
                    } else if(entity->getXVelocity() == 0) {
                        entity->setXVelocity(getXVelocity());
                    }
                }
            }

            justHitPlatform = true;
        }
    } else {
        justHitPlatform = false;
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