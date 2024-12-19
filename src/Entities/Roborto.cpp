#include <cmath>

#include "../../includes/Entities/Roborto.h"

void Roborto::pathFind(int x, int y, int& leftWeight, int& rightWeight, GlobalGameState& ggs, Level& level, bool firstCall = false) {
    if(y < ggs.playerTileY) {
        // Find right weight
        int rightEdge = findEdgeRight(x,y,level);
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
                if(y+numberTilesDownRight < level.getMap().size() && level.getMap()[y+numberTilesDownRight][newXTile] == -1) {
                    numberTilesDownRight++;
                    float dt = (-v+sqrt(2*ACCELERATION*TILE_SIZE_SCALED+ pow(v,2)))/ACCELERATION;
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

            pathFind(newXTile,y+numberTilesDownRight, tempRRightWeight, tempRLeftWeight, ggs, level);

            if(tempRLeftWeight >= tempRRightWeight) {
                rightWeight = tempRRightWeight + rightEdge;
            } else {
                rightWeight = tempRLeftWeight + rightEdge;
            }
        }


        // Find left weight
        int leftEdge = findEdgeLeft(x,y,level);
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
                if(y+numberTilesDownLeft < level.getMap().size() && level.getMap()[y+numberTilesDownLeft][newXTile] == -1) {
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

            pathFind(newXTile,y+numberTilesDownLeft, tempLLeftWeight, tempLRightWeight, ggs, level);

            if(tempLLeftWeight >= tempLRightWeight) {
                leftWeight = tempLRightWeight + leftEdge;
            } else {
                leftWeight = tempLLeftWeight + leftEdge;
            }
        }
    } else if(y == ggs.playerTileY) {
        int xTileDifference = ggs.playerTileX-x;
        int xDifference = firstCall ? ggs.playerX - entity->getRect().x : xTileDifference;
        int direction = 1;
        if(xDifference != 0) {
            direction = xDifference/abs(xDifference);
        }

        bool allPlatforms = true;
        if(ggs.playerTileX != x-1 || ggs.playerTileX != x+1) {
            for(int i = 1; i < abs(ggs.playerTileX - x); i++) {
                if(y < level.getMap().size() && level.getMap()[y][x+(i*direction)] == -1) {
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

void Roborto::move(GlobalGameState& ggs, const std::list<Platform>& platforms, Level& level) {
    if(!entity->move(ggs.dt,platforms)) {
        if(!justHitPlatform) {
            int tileX = entity->getRect().x/TILE_SIZE_SCALED;
            int tileY = entity->getRect().y/TILE_SIZE_SCALED;
            if(tileY < level.getMap().size() && level.getMap()[tileY][tileX] != -1) {
                int leftWeight;
                int rightWeight;
                pathFind(tileX,tileY,leftWeight,rightWeight,ggs, level, true);
                if(leftWeight >= 1000 && rightWeight >= 1000) {
                    int numberTilesRight = findEdgeRight(tileX,tileY,level);
                    int numberTilesLeft = findEdgeLeft(tileX,tileY,level);

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

    if(entity->getRect().y >= level.getMap().size()*TILE_SIZE_SCALED) {
        entity->despawn();
    }

    //TODO: Move to entity
    if(entity->getRect().x >= WINDOW_WIDTH) {
        entity->setPosition(scale(10),entity->getRect().y);
    } else if(entity->getRect().x <= 0) {
        entity->setPosition(WINDOW_WIDTH-scale(10), entity->getRect().y);
    }


}