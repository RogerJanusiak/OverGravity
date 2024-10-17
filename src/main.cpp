#include <cmath>
#include <fstream>
#include <list>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <vector>
#include <SDL_mixer.h>
#include <filesystem>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"
#include "../includes/State.h"

#include <SDL_ttf.h>

#include "../includes/Sound.h"
#include "../includes/UI.h"

SDL_Window *gameWindow = nullptr;
SDL_Renderer *gameRenderer = nullptr;

std::string gameFilesPath;

//Game Controller 1 handler
SDL_GameController* controller;

bool init();
void close();
void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::list<Entity*>& eRobots);
void renderPlatforms(std::list<Platform*>& platforms);
std::list<Entity> getWaveEnemyEntities(int waveNumber,int divisor, std::vector<Spawn>* spawns);
void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns);
void loadController();

void log(int x) {
    SDL_Log(std::to_string(x).c_str());
}

void moveCamera(int x, int y, std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns);

bool loadValuesFromCSV(std::string &filePath);


bool pauseEnemy = false;

int bulletSpeed = 1;
int timpyXVelocity = 1;
int roborXVelocity = 1;
double revolverReloadSpeed = 1;
int comboToGetShield = 1;
double abilityChargeSpeed = 15;

int levelHeight = 0;

std::vector<std::vector<int>> map;

Player* timpyPointer = nullptr;

State state = { true, false, false, false, 0,0, false };

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {
        bool quit = false;

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        std::list<Platform> ePlatforms;
        std::vector<Spawn> enemySpawns;
        enemySpawns.reserve(15);
        std::vector<Spawn> playerSpawns;
        playerSpawns.reserve(2);

        std::vector<Spawn*> allSpawns;
        allSpawns.reserve(15);
        std::list<Platform*> platforms;

        std::list<Entity> eBullets;
        std::list<Bullet> bullets;

        Entity eTimpy = Entity(&playerSpawns,gameRenderer);
        Player timpy = Player(&eTimpy);
        timpyPointer = &timpy;

        bool leftMovement = false;
        bool rightMovement = false;

        bool waveOverride = false;

        bool shootingReset = true;

        bool inWave;
        int waveNumber = 0;

        float lastFPS = 0;

        UI_init(gameRenderer);
        initStartScreen(controller != nullptr);

        Sound pistolReload("resources/sounds/pistolReload.wav", 0);
        Sound explosion("resources/sounds/explosion.wav", 0);
        Sound song("resources/sounds/song.wav", -1);

        //Game Loop
        while(!quit) {

            song.play();

            while(!state.started && !quit) {
                SDL_RenderClear(gameRenderer);

                while(SDL_PollEvent(&e) != 0) {
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    } else if( e.type == SDL_KEYDOWN ) {
                        if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2) {
                            state.started = true;
                        }
                    } else if( e.type == SDL_JOYBUTTONDOWN ) {
                        if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                            state.started = true;
                        }
                    } else if(e.type == SDL_JOYDEVICEADDED ) {
                        loadController();
                        initStartScreen(controller != nullptr);
                        renderStartScreen(state);
                    } else if (e.type == SDL_JOYDEVICEREMOVED) {
                        controller = nullptr;
                        initStartScreen(controller != nullptr);
                        renderStartScreen(state);
                    } else  if( e.type == SDL_MOUSEMOTION) {
                        mouseMove(state);
                    } else if(e.type == SDL_MOUSEBUTTONDOWN) {
                        mouseClick(state);
                    }
                }

                renderStartScreen(state);
                SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
                SDL_RenderPresent(gameRenderer);
            }

            if(!state.initialLoad) {
                char* appDir = SDL_GetBasePath();
                std::string currentPath(appDir);
                SDL_free(appDir);
                std::string levelPath;

                if(state.level == 1) {
                    levelPath = currentPath + "resources/levels/level1.csv";
                } else if(state.level == 2) {
                    levelPath = currentPath + "resources/levels/level2.csv";
                }

                loadLevelFromCSV((levelPath), ePlatforms, enemySpawns, playerSpawns);

                for(auto it = enemySpawns.begin(); it != enemySpawns.end(); ++it) {
                    allSpawns.push_back(&*it);
                }
                for(auto it = playerSpawns.begin(); it != playerSpawns.end(); ++it) {
                    allSpawns.push_back(&*it);
                }

                for(auto it = ePlatforms.begin(); it != ePlatforms.end(); ++it) {
                    platforms.push_back(&*it);
                }

                timpy.getEntity()->forceSpawn();
                state.initialLoad = true;
            }

            inWave = true;
            waveNumber++;

            std::list<Entity> tempRobors = getWaveEnemyEntities(waveNumber,1, &enemySpawns);
            std::list<Entity> eRobots;
            std::list<Robor> robors;
            std::list<Entity*> allCharacterEntities;

            allCharacterEntities.push_back(timpy.getEntity());

            for (auto it = tempRobors.begin(); it != tempRobors.end(); ++it) {
                eRobots.push_back(*it);
                robors.emplace_back(&(*it),roborXVelocity);
                allCharacterEntities.emplace_back(&(*it));
            }

            bool waveStarted = false;
            Uint32 startWaveLoad = SDL_GetTicks();

            checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

            while(inWave && !quit) {
                Uint64 start = SDL_GetPerformanceCounter();

                //Controls Loop
                while(SDL_PollEvent(&e) != 0) {
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    } else if( e.type == SDL_KEYDOWN ) {
                        if(e.key.keysym.sym == SDLK_1) {
                            state.developerMode = !state.developerMode;
                        }
                        if(e.key.keysym.sym == SDLK_2 && state.developerMode) {
                            waveNumber = 30;
                            waveOverride = true;
                        }
                        if(e.key.keysym.sym == SDLK_3 && state.developerMode) {
                            moveCamera(0,-1*state.camY,allCharacterEntities,platforms,allSpawns);
                            timpy.getEntity()->forceSpawn();
                        }
                        if(e.key.keysym.sym == SDLK_4 && state.developerMode) {
                           pauseEnemy = !pauseEnemy;
                        }
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
                        }
                        if(e.key.keysym.sym == SDLK_j) {
                            timpy.setDirection(false);
                        }
                        if(e.key.keysym.sym == SDLK_w) {
                            timpy.changeWeapon();
                        }
                        if(e.key.keysym.sym == SDLK_SPACE && waveStarted) {
                            if(shootingReset) {
                                timpy.shoot(&eBullets,&bullets,bulletSpeed);
                                shootingReset = false;
                            }
                        } else if(e.key.keysym.sym == SDLK_e) {
                            if(timpy.useAbility()) {
                                moveCamera(0,-1*state.camY,allCharacterEntities,platforms,allSpawns);
                                timpy.getEntity()->forceSpawn();
                            }
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

                        if(e.key.keysym.sym == SDLK_SPACE) {
                            shootingReset = true;
                        }
                    } else if( e.type == SDL_JOYAXISMOTION && waveStarted) {
                        if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                            if(shootingReset) {
                                timpy.shoot(&eBullets,&bullets,bulletSpeed);
                                shootingReset = false;
                            }
                        } else {
                            shootingReset = true;
                        }

                        if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) > JOYSTICK_DEAD_ZONE) {
                            timpy.setDirection(true);
                        } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) < -JOYSTICK_DEAD_ZONE) {
                            timpy.setDirection(false);
                        }

                        if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
                            timpy.getEntity()->setXVelocity(timpyXVelocity);
                        } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
                            timpy.getEntity()->setXVelocity(-timpyXVelocity);
                        } else {
                            timpy.getEntity()->setXVelocity(0);
                        }
                    } else if(e.type == SDL_JOYDEVICEADDED ) {
                         loadController();
                    } else if (e.type == SDL_JOYDEVICEREMOVED) {
                        controller = nullptr;
                    }
                }

                Uint32 current = SDL_GetTicks();
                float dt = (current - lastUpdate) / 1000.0f;
                lastUpdate = current;

                if((current-startWaveLoad)/1000.0f > 1) {
                    waveStarted = true;
                }
                updateTimeToShoot(scale(timpy.reload(dt,revolverReloadSpeed)));
                updateTimeToAbility(scale(timpy.charge(dt,abilityChargeSpeed)));
                if (timpy.wasJustReloaded()) {
                    pistolReload.play();
                }

                SDL_RenderClear(gameRenderer);

                if(state.developerMode) {
                    for(auto spawn : allSpawns) {
                        if(!spawn->getOccupied()) {
                            SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                        } else {
                            SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                        }
                        SDL_RenderDrawRect(gameRenderer, &spawn->getRect());
                    }
                } else {
                    for(auto spawn : allSpawns) {
                        spawn->render(gameRenderer);
                    }
                }

                bool robotAlive = false;
                bool playerAlive = true;

                //Render/Move Bullets
                for (auto it = bullets.begin(); it != bullets.end();) {
                    if(it->move(dt, platforms, state.developerMode)) {
                        eBullets.erase(it->getIterator());
                        it = bullets.erase(it);
                    } else {
                        it->render();
                        ++it;
                    }
                }

                //Render/Move/Collision Enemies
                bool playerDamaged = false;
                SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                for (auto it = robors.begin(); it != robors.end();++it) {
                    bool firstLoop = false;
                    if(!it->getEntity()->isSpawned()) {
                        it->getEntity()->spawn();
                        firstLoop = true;
                    }
                    if(it->alive && it->getEntity()->isSpawned()) {
                        if(!firstLoop && waveStarted && !pauseEnemy) {
                            it->move(dt, platforms,state.camY,levelHeight);
                        }
                        it->render();
                        if(timpy.getWeapon() == Weapon::knife && Entity::isColliding(it->getEntity()->getRect(),timpy.getWeaponRect())) {
                            it->alive = false;
                            explosion.play();
                        } else {
                            if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                if(timpy.getEntity()->getRect().y + (timpy.getEntity()->getRect().h-it->getEntity()->getRect().h) < it->getEntity()->getRect().y) {
                                    timpy.getEntity()->setYVelocity(-1800);
                                    explosion.play();
                                } else {
                                    if(timpy.damage()) {
                                        playerAlive = false;
                                        waveNumber = 0;
                                        timpy.zeroCombo();
                                        updateInGameText(timpy.getCombo(),waveNumber);
                                    }
                                    playerDamaged = true;
                                }
                                it->alive = false;
                            }
                        }
                        for(auto bit = bullets.begin(); bit != bullets.end();) {
                            if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                eBullets.erase(bit->getIterator());
                                bit = bullets.erase(bit);
                                timpy.increaseCombo(comboToGetShield);
                                it->alive = false;
                                explosion.play();
                            } else {
                                ++bit;
                            }
                        }
                        updateInGameText(timpy.getCombo(),waveNumber);
                        if(state.developerMode) {
                            SDL_RenderDrawRect(gameRenderer,&it->getEntity()->getRect());
                        }
                    }
                    if(it->alive) {
                        robotAlive = true;
                    }
                }

                if(playerDamaged) {
                    timpy.zeroCombo();
                    updateInGameText(timpy.getCombo(),waveNumber);
                    timpy.charge(abilityChargeSpeed,abilityChargeSpeed);
                    moveCamera(0,-1*state.camY,allCharacterEntities,platforms,allSpawns);
                    timpy.getEntity()->forceSpawn();
                }

                if(!playerAlive || waveOverride) {
                    inWave = false;
                    waveOverride = false;
                } else {
                    inWave = robotAlive;
                }

                if(timpy.getEntity()->isSpawned()) {
                    if(waveStarted) {

                        //TODO: Check if moveCamera will over shoot and then set it to max.
                        if(timpy.getEntity()->getRect().y >= scale(195) && state.camY > -1*(scale(levelHeight)-WINDOW_HEIGHT)) {
                            int movmentDistance = timpy.move(dt, platforms,state.camY);
                            if(movmentDistance < 0) {
                                moveCamera(0,movmentDistance,allCharacterEntities,platforms,allSpawns);
                            }
                        } else {
                            timpy.move(dt, platforms,state.camY);
                            if(timpy.getEntity()->getRect().y > WINDOW_HEIGHT) {
                                moveCamera(0,-1*state.camY,allCharacterEntities,platforms,allSpawns);
                                timpy.getEntity()->forceSpawn();
                            } else if (state.camY < -1*(scale(levelHeight)-WINDOW_HEIGHT)) {
                                moveCamera(0,-1*(state.camY+scale(levelHeight)-WINDOW_HEIGHT),allCharacterEntities,platforms,allSpawns);
                            }
                         }
                    }
                    timpy.render();
                }

                SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                if(state.developerMode) {
                    SDL_RenderDrawRect(gameRenderer,&timpy.getEntity()->getRect());
                    SDL_RenderDrawRect(gameRenderer, timpy.getWheelRect());
                }

                checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                renderPlatforms(platforms);

                renderInGameText(state.developerMode, lastFPS, waveStarted);

                renderPlayerUI(&timpy);

                SDL_SetRenderDrawColor(gameRenderer, 16, 16, 16, 255);
                SDL_RenderPresent(gameRenderer);

                Uint64 end = SDL_GetPerformanceCounter();

                float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
                lastFPS = (1.0f / elapsed);
            }
        }

    }
    close();
    return 0;
}

void renderPlatforms(std::list<Platform*>& platforms) {
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
    for (auto it = platforms.begin(); it != platforms.end(); ++it) {
        (*it)->render();
        if(state.developerMode) {
            SDL_RenderDrawRect(gameRenderer,&(*it)->getPlatformRect());
        }
    }
}

void moveCamera(int x, int y, std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns) {
    state.camY += y;

    for (auto entites : allCharacterEntities) {
        entites->setPosition(entites->getRect().x+x,entites->getRect().y+y);
    }
    timpyPointer->updateWheelRect();
    for (auto platform : platforms) {
        platform->setPosition(platform->getPlatformRect().x+x,platform->getPlatformRect().y+y);
    }
    for(auto spawn : allSpawns) {
        spawn->setPosition(spawn->getRect().x+x,spawn->getRect().y+y);
    }
}

void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::list<Entity*>& allCharacterEntities) {
    for (auto sit = allSpawns.begin(); sit != allSpawns.end(); ++sit) {
        (*sit)->setOccupied(false);
        for (auto it = allCharacterEntities.begin(); it != allCharacterEntities.end(); ++it) {
            if(Entity::isColliding((*sit)->getRect(),(*it)->getRect())) {
                (*sit)->setOccupied(true);
            }
        }
    }
}

std::list<Entity> getWaveEnemyEntities(const int waveNumber,const int divisor, std::vector<Spawn>* spawns) {
    std::list<Entity> entities;
    for(int i = 1; i <= waveNumber; i++) {
        if(i % divisor == 0) {
            entities.emplace_back(spawns,gameRenderer);
        }
    }
    return entities;
}

void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns) {
    std::ifstream file((filePath).c_str());
    if (!file.is_open()) {
        SDL_Log("Could not load level file!");
    }
    constexpr int MAX_ROWS = 100;
    constexpr int MAX_COLS = 12;
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
    levelHeight = row*TILE_SIZE;
    for (int i = 0; i < row; i++) {
        std::vector<int> mapRow;
        for(int j = 0; j < MAX_COLS; j++) {
            int multiplier = j-1;
            if(std::stoi(data[i][j]) == 0) {
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(0);
            } else if(std::stoi(data[i][j]) == 1) {
                playerSpawns.emplace_back(scale(multiplier*TILE_SIZE-25),scale(i*TILE_SIZE+(TILE_SIZE-17-60)),scale(50),scale(60),1);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(1);
            } else if(std::stoi(data[i][j]) == 2) {
                enemySpawns.emplace_back(scale(multiplier*TILE_SIZE+(TILE_SIZE-50)/2),scale(i*TILE_SIZE+(TILE_SIZE-17-50)),scale(50),scale(50),2);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),gameRenderer);
                mapRow.push_back(2);
            } else {
                mapRow.push_back(-1);
            }
        }
        map.push_back(mapRow);
    }
}

bool loadValuesFromCSV(std::string &filePath) {
    std::ifstream file((filePath).c_str());
    if (!file.is_open()) {
        SDL_Log("Could not load values file!");
        return false;
    }
    constexpr int MAX_ROWS = 5;
    constexpr int MAX_COLS = 2;
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
    for (int i = 0; i < row; i++) {
        if(std::stoi(data[i][0]) == 0) {
            bulletSpeed = scale(std::stoi(data[i][1]));
        }
        if(std::stoi(data[i][0]) == 1) {
            timpyXVelocity = scale(std::stoi(data[i][1]));
        }
        if(std::stoi(data[i][0]) == 2) {
            roborXVelocity = scale(std::stoi(data[i][1]));
        }
        if(std::stoi(data[i][0]) == 3) {
            revolverReloadSpeed = std::stod(data[i][1]);
        }
        if(std::stoi(data[i][0]) == 4) {
            comboToGetShield = std::stoi(data[i][1]);
        }
    }
    return true;
}

void loadController() {
    controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
    if(controller == nullptr) {
        SDL_Log( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
    } else {
        SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
    }
}

bool init() {
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO ) < 0 ) {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else if (TTF_Init() < 0) {
        SDL_Log( "TTF could not initialize! SDL Error: %s\n", SDL_GetError() );
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

        loadController();
        char* basePath = SDL_GetBasePath();
        std::string currentPath(basePath);
        SDL_free(basePath);
        std::string valuesPath = currentPath + "resources/values.csv";
        if(!loadValuesFromCSV(valuesPath)) {
            SDL_Log("Could not load values from value file!");
            success = false;
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }

    }

    return success;
}

void close() {
    SDL_DestroyRenderer(gameRenderer);
    gameRenderer = nullptr;

    SDL_GameControllerClose(controller);
    controller = nullptr;

    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
