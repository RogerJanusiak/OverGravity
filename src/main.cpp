#include <fstream>
#include <iostream>
#include <list>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <vector>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/Texture.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"

class Spawn;
SDL_Window *gameWindow = nullptr;
SDL_Renderer *gameRenderer = nullptr;

bool isColliding(SDL_Rect a, SDL_Rect b);

bool init() {
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        gameWindow = SDL_CreateWindow("OverGravity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (gameWindow == nullptr) {
            SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        } else {
            gameRenderer = SDL_CreateRenderer(gameWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gameRenderer == nullptr) {
                SDL_Log("Game renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

void close() {
    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}

std::vector<Entity> getEntities(const int waveNumber,const int divisor) {
    std::vector<Entity> etities;
    for(int i = 1; i <= waveNumber; i++) {
        if(i % divisor == 0) {
            etities.emplace_back(10,40,350,0,gameRenderer);
        }
    }
    return etities;
}

void loadLevel(std::string filePath,std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, Spawn& playerSpawn) {
    std::ifstream file(("../resources/levels/" + filePath).c_str());
    if (!file.is_open()) {
        SDL_Log("Could not load level file!");
    }
    const int MAX_ROWS = 100;
    const int MAX_COLS = 3;
    std::string data[MAX_ROWS][MAX_COLS];
    std::string line;
    int row = 0;
    // Store the CSV data from the CSV file to the 2D array
    while (getline(file, line) && row < MAX_ROWS) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (getline(ss, cell, ',') && col < MAX_COLS) {
            data[row][col] = cell;
            col++;
        }
        row++;
    }
    file.close();

    // Print the data stored in the 2D array
    for (int i = 0; i < row; i++) {
        if(stoi(data[i][0]) == 0) {
            platforms.emplace_back(stoi(data[i][1]),stoi(data[i][2])+25,gameRenderer);
        }
        if(stoi(data[i][0]) == 1) {
            playerSpawn.init(stoi(data[i][1]),stoi(data[i][2])+25,54,54);
        }
        if(stoi(data[i][0]) == 2) {
            enemySpawns.emplace_back(stoi(data[i][1]),stoi(data[i][2])+25,54,54);
        }
    }
}

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {
        bool quit = false;

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        std::list<Platform> ePlatforms;
        std::vector<Spawn> enemySpawns;
        Spawn playerSpawn;

        loadLevel("level1.csv", ePlatforms, enemySpawns, playerSpawn);

        std::list<Platform*> platforms;
        for(auto it = ePlatforms.begin(); it != ePlatforms.end(); it++) {
            platforms.push_back(&*it);
        }

        std::list<Entity> eBullets;
        std::list<Bullet> bullets;

        Entity eTimpy = Entity(playerSpawn.getX(),playerSpawn.getY(),0,0,gameRenderer);
        Player timpy = Player(&eTimpy, playerSpawn);

        bool leftMovement = false;
        bool rightMovement = false;

        bool leftFacing = false;
        bool rightFacing = false;

        int timpyXVelocity = 400;
        bool canShoot = true;

        float lastShotTimeDifference = 0;

        bool inWave = true;
        int waveNumber = 30;

        //Game Loop
        while(!quit) {
            inWave = true;
            waveNumber++;
            std::vector<Entity> eRobots = getEntities(waveNumber,1);
            std::vector<Robor> robors;
            for (auto it = eRobots.begin(); it != eRobots.end(); it++) {
                robors.emplace_back(&(*it));
            }
            int currentEnemySpawn = 0;

            while(inWave && !quit) {
                //Controls Loop
                while(SDL_PollEvent(&e) != 0) {
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    } else if( e.type == SDL_KEYDOWN ) {
                        if(e.key.keysym.sym == SDLK_d) {
                            timpy.getEntity()->setXVelocity(timpyXVelocity);
                            rightMovement = true;
                        }
                        if(e.key.keysym.sym == SDLK_a) {
                            timpy.getEntity()->setXVelocity(-timpyXVelocity);
                            leftMovement = true;
                        }
                        if(e.key.keysym.sym == SDLK_l) {
                            timpy.setDirection(true);
                            rightFacing = true;
                            leftFacing = false;
                        }
                        if(e.key.keysym.sym == SDLK_j) {
                            timpy.setDirection(false);
                            leftFacing = true;
                            rightFacing = false;
                        }
                        if(e.key.keysym.sym == SDLK_w) {
                            timpy.changeWeapon();
                        }

                    } else if(e.type == SDL_KEYUP) {
                        if(e.key.keysym.sym == SDLK_d)
                            rightMovement = false;
                        if(e.key.keysym.sym == SDLK_a)
                            leftMovement = false;
                        if(!leftMovement && !rightMovement) {
                            timpy.getEntity()->setXVelocity(0);
                        } else if (leftMovement) {
                            timpy.getEntity()->setXVelocity(-timpyXVelocity);
                        } else {
                            timpy.getEntity()->setXVelocity(timpyXVelocity);
                        }

                        if(e.key.keysym.sym == SDLK_l) {
                            rightFacing = false;
                        }
                        if(e.key.keysym.sym == SDLK_j) {
                            leftFacing = false;
                        }


                    }
                }

                if(rightFacing && canShoot && timpy.getWeapon() == 1) {
                    eBullets.emplace_back(timpy.getEntity()->getRect().x+90,timpy.getEntity()->getRect().y+28,1000,0,gameRenderer);
                    bullets.emplace_back(&eBullets.back());
                    bullets.back().setIterator(--eBullets.end());
                    canShoot = false;
                } else if(leftFacing && canShoot && timpy.getWeapon() == 1) {
                    eBullets.emplace_back(timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y+28,-1000,0,gameRenderer);
                    bullets.emplace_back(&eBullets.back());
                    bullets.back().setIterator(--eBullets.end());
                    canShoot = false;
                }

                Uint32 current = SDL_GetTicks();
                float dt = (current - lastUpdate) / 1000.0f;
                lastUpdate = current;

                if (lastShotTimeDifference > 1) {
                    lastShotTimeDifference = 0;
                    canShoot = true;
                } else if(!canShoot) {
                    lastShotTimeDifference += dt;
                }

                SDL_RenderClear(gameRenderer);

                SDL_SetRenderDrawColor(gameRenderer, 16, 16, 16, 255);
                for (auto platform : platforms)
                    platform->render();

                const SDL_Rect* playerRect = &timpy.getEntity()->getRect();

                bool robotAlive = false;
                bool playerAlive = true;

                for (auto it = bullets.begin(); it != bullets.end(); it++) {
                    it->render();
                    if(it->move(dt)) {
                        eBullets.erase(it->getIterator());
                        bullets.erase(it);
                    }
                }

                for (auto it = robors.begin(); it != robors.end(); it++) {
                    if(!it->getEntity()->isSpawned()) {
                        if(!enemySpawns[currentEnemySpawn].getOccupied()) {
                            it->getEntity()->setSpawned(true);
                            it->getEntity()->setPosition(enemySpawns[currentEnemySpawn].getX(),enemySpawns[currentEnemySpawn].getX());
                            robotAlive = true;
                            enemySpawns[currentEnemySpawn].setOccupied(true);
                            if (currentEnemySpawn < enemySpawns.size()-1) {
                                currentEnemySpawn++;
                            } else {
                                currentEnemySpawn = 0;
                            }
                        }
                    }
                    if(it->alive && it->getEntity()->isSpawned()) {
                        robotAlive = true;
                        const SDL_Rect* robotRect = &it->getEntity()->getRect();

                        it->move(dt, platforms);
                        it->render();
                        if(timpy.getWeapon() == 2 && Entity::isColliding(it->getEntity()->getRect(),timpy.getWeaponRect())) {
                            it->alive = false;
                        } else {
                            if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                playerAlive = false;
                                waveNumber = 1;
                                timpy.getEntity()->setPosition(timpy.getSpawn().getX(),timpy.getSpawn().getY());
                            }
                        }

                        for(auto bit = bullets.begin(); bit != bullets.end(); bit++) {
                            if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                it->alive = false;
                                eBullets.erase(bit->getIterator());
                                bullets.erase(bit);
                            }
                        }
                        for(auto enemySpawn : enemySpawns) {
                            if(Entity::isColliding(enemySpawn.getRect(),it->getEntity()->getRect())) {
                                enemySpawn.setOccupied(true);
                            }
                        }
                    }
                }
                if(!playerAlive) {
                    inWave = false;
                } else {
                    inWave = robotAlive;
                }

                timpy.move(dt, platforms);
                timpy.render();

                SDL_RenderPresent(gameRenderer);
            }

        }
    }

    close();

    return 0;
}
