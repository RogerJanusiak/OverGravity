#include <cmath>
#include <fstream>
#include <list>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <vector>
#include <filesystem>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"

#include <SDL_ttf.h>

#include "../includes/UI.h"

SDL_Window *gameWindow = nullptr;
SDL_Renderer *gameRenderer = nullptr;

std::string gameFilesPath;

//Game Controller 1 handler
SDL_GameController* controller;

bool init();
void close();
void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::vector<Entity*>& allCharacterEntities);
void renderPlatforms(std::list<Platform*>& platforms);
std::vector<Entity> getWaveEnemyEntities(int waveNumber,int divisor, std::vector<Spawn>* spawns);
void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns);
void loadController();

bool loadValuesFromCSV(std::string &filePath);

bool developerMode = false;
bool started = false;
bool multiplayer = false;

int bulletSpeed = 1;
int timpyXVelocity = 1;
int roborXVelocity = 1;
double revolverReloadSpeed = 1;
int comboToGetShield = 1;

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {
        bool quit = false;

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        std::list<Platform> ePlatforms;
        std::vector<Spawn> enemySpawns;
        std::vector<Spawn> playerSpawns;

        char* appDir = SDL_GetBasePath();
        std::string currentPath(appDir);
        SDL_free(appDir);
        std::string levelPath = currentPath + "resources/levels/level1.csv";
        loadLevelFromCSV((levelPath), ePlatforms, enemySpawns, playerSpawns);

        std::vector<Spawn*> allSpawns;
        for(auto it = enemySpawns.begin(); it != enemySpawns.end(); ++it) {
            allSpawns.push_back(&*it);
        }
        for(auto it = playerSpawns.begin(); it != playerSpawns.end(); ++it) {
            allSpawns.push_back(&*it);
        }

        std::list<Platform*> platforms;
        for(auto it = ePlatforms.begin(); it != ePlatforms.end(); ++it) {
            platforms.push_back(&*it);
        }

        std::list<Entity> eBullets;
        std::list<Bullet> bullets;

        Entity eTimpy = Entity(&playerSpawns,gameRenderer);
        Player timpy = Player(&eTimpy);
        timpy.getEntity()->spawn();

        bool leftMovement = false;
        bool rightMovement = false;

        bool waveOverride = false;

        bool canShoot = true;
        bool shootingReset = true;
        float lastShotTimeDifference = 0;

        bool inWave;
        int waveNumber = 0;

        float lastFPS = 0;

        UI_init(gameRenderer);
        initStartScreen(controller != nullptr);

        //Game Loop
        while(!quit) {

            while(!started && !quit) {
                SDL_RenderClear(gameRenderer);

                while(SDL_PollEvent(&e) != 0) {
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    } else if( e.type == SDL_KEYDOWN ) {
                        if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2) {
                            started = true;
                        }
                    } else if( e.type == SDL_JOYBUTTONDOWN ) {
                        if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                            started = true;
                        }
                    } else if(e.type == SDL_JOYDEVICEADDED ) {
                        loadController();
                        initStartScreen(controller != nullptr);
                    } else if (e.type == SDL_JOYDEVICEREMOVED) {
                        controller = nullptr;
                        initStartScreen(controller != nullptr);
                    }
                }

                renderStartScreen();
                SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
                SDL_RenderPresent(gameRenderer);
            }

            inWave = true;
            waveNumber++;
            std::vector<Entity> eRobots = getWaveEnemyEntities(waveNumber,1, &enemySpawns);
            std::vector<Robor> robors;
            std::vector<Entity*> allCharacterEntities;
            allCharacterEntities.push_back(timpy.getEntity());

            for (auto it = eRobots.begin(); it != eRobots.end(); ++it) {
                robors.emplace_back(&(*it),roborXVelocity);
                allCharacterEntities.push_back(&(*it));
            }
            timpy.getEntity()->spawn();

            bool waveStarted = false;
            Uint32 startWaveLoad = SDL_GetTicks();

            while(inWave && !quit) {

                Uint64 start = SDL_GetPerformanceCounter();

                //Controls Loop
                while(SDL_PollEvent(&e) != 0) {
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    } else if( e.type == SDL_KEYDOWN ) {
                        if(e.key.keysym.sym == SDLK_1) {
                            developerMode = !developerMode;
                        }
                        if(e.key.keysym.sym == SDLK_2) {
                            waveNumber = 30;
                            waveOverride = true;
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

                SDL_RenderClear(gameRenderer);

                bool robotAlive = false;
                bool playerAlive = true;

                //Render/Move Bullets
                for (auto it = bullets.begin(); it != bullets.end();) {
                    if(it->move(dt, platforms, developerMode)) {
                        eBullets.erase(it->getIterator());
                        it = bullets.erase(it);
                    } else {
                        it->render();
                        ++it;
                    }
                }

                //Render/Move/Collision Enemies
                SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                for (auto it = robors.begin(); it != robors.end(); ++it) {
                    bool firstLoop = false;
                    if(!it->getEntity()->isSpawned()) {
                        it->getEntity()->spawn();
                        firstLoop = true;
                    }
                    if(it->alive && it->getEntity()->isSpawned()) {
                        robotAlive = true;

                        if(!firstLoop && waveStarted) {
                            it->move(dt, platforms);
                        }
                        it->render();
                        if(timpy.getWeapon() == Weapon::knife && Entity::isColliding(it->getEntity()->getRect(),timpy.getWeaponRect())) {
                            it->alive = false;
                        } else {
                            if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                if(timpy.getShield() == 2) {
                                    timpy.decreaseShield();
                                    timpy.getEntity()->despawn();
                                    timpy.getEntity()->spawn();
                                    timpy.zeroCombo();
                                    updateInGameText(timpy.getCombo(),waveNumber);
                                } else if(timpy.getShield() == 1) {
                                    timpy.decreaseShield();
                                    timpy.getEntity()->despawn();
                                    timpy.getEntity()->spawn();
                                    timpy.zeroCombo();
                                    updateInGameText(timpy.getCombo(),waveNumber);
                                } else {
                                    if(timpy.getHP() == 2) {
                                        timpy.damage();
                                        timpy.getEntity()->despawn();
                                        timpy.getEntity()->spawn();
                                        timpy.zeroCombo();
                                        updateInGameText(timpy.getCombo(),waveNumber);
                                    } else if (timpy.getHP() == 1) {
                                        playerAlive = false;
                                        waveNumber = 0;
                                        timpy.setHP(2);
                                        timpy.getEntity()->despawn();
                                        timpy.zeroCombo();
                                        updateInGameText(timpy.getCombo(),waveNumber);
                                    }
                                }
                            }
                        }
                        for(auto bit = bullets.begin(); bit != bullets.end();) {
                            if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                //TODO: Does this despawn the enemy type?
                                //TODO: This should probably erase the enemy
                                //TODO: Check behaivor when enemy goes off map it doesn't end the wave
                                it->alive = false;
                                eBullets.erase(bit->getIterator());
                                bit = bullets.erase(bit);
                                timpy.increaseCombo(comboToGetShield);
                            } else {
                                ++bit;
                            }
                        }
                        updateInGameText(timpy.getCombo(),waveNumber);
                        if(developerMode) {
                            SDL_RenderDrawRect(gameRenderer,&it->getEntity()->getRect());
                        }
                    }
                }

                if(!playerAlive || waveOverride) {
                    inWave = false;
                    waveOverride = false;
                }
                else {
                    inWave = robotAlive;
                }

                if(timpy.getEntity()->isSpawned()) {
                    if(waveStarted) {
                        timpy.move(dt, platforms);
                    }
                    timpy.render();
                }

                SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                if(developerMode) {
                    SDL_RenderDrawRect(gameRenderer,&timpy.getEntity()->getRect());
                }

                checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                renderPlatforms(platforms);

                renderInGameText(developerMode, lastFPS, waveStarted);

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
        if(developerMode) {
            SDL_RenderDrawRect(gameRenderer,&(*it)->getPlatformRect());
        }
    }
}

void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::vector<Entity*>& allCharacterEntities) {
    for (auto sit = allSpawns.begin(); sit != allSpawns.end(); ++sit) {
        (*sit)->setOccupied(false);
        for (auto it = allCharacterEntities.begin(); it != allCharacterEntities.end(); ++it) {
            if(Entity::isColliding((*sit)->getRect(),(*it)->getRect())) {
                (*sit)->setOccupied(true);
            }
        }
    }
}

std::vector<Entity> getWaveEnemyEntities(const int waveNumber,const int divisor, std::vector<Spawn>* spawns) {
    std::vector<Entity> entities;
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
    constexpr int MAX_COLS = 3;
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
            platforms.emplace_back(std::stoi(data[i][1]),std::stoi(data[i][2]),gameRenderer);
        }
        if(std::stoi(data[i][0]) == 1) {
            playerSpawns.emplace_back(scale(std::stoi(data[i][1])),scale(std::stoi(data[i][2])),scale(50),scale(60));
        }
        if(std::stoi(data[i][0]) == 2) {
            enemySpawns.emplace_back(scale(std::stoi(data[i][1])),scale(std::stoi(data[i][2])),scale(50),scale(50));
        }
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

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 ) {
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

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
