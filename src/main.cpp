#include <fstream>
#include <list>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <vector>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"

#include <windows.h>
#include <shlobj.h>
#include <SDL_ttf.h>

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
void loadLevelFromCSV(std::string filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns);

bool developerMode = false;
bool useController = false;
bool started = false;

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {
        bool quit = false;

        SDL_Rect timeToShootBack;
        SDL_Rect timeToShoot;

        timeToShootBack.x = WINDOW_WIDTH-90*SCALE_FACTOR;
        timeToShootBack.y = WINDOW_HEIGHT-25*SCALE_FACTOR;
        timeToShootBack.w = 75*SCALE_FACTOR;
        timeToShootBack.h = 15*SCALE_FACTOR;

        timeToShoot.x = WINDOW_WIDTH-90*SCALE_FACTOR;
        timeToShoot.y = WINDOW_HEIGHT-25*SCALE_FACTOR;
        timeToShoot.w = 50*SCALE_FACTOR;
        timeToShoot.h = 15*SCALE_FACTOR;

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        std::list<Platform> ePlatforms;
        std::vector<Spawn> enemySpawns;
        std::vector<Spawn> playerSpawns;

        loadLevelFromCSV((gameFilesPath + "\\level1.csv"), ePlatforms, enemySpawns, playerSpawns);

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

        int timpyXVelocity = 350*SCALE_FACTOR;

        bool canShoot = true;
        bool shootingReset = true;
        float lastShotTimeDifference = 0;

        bool inWave;
        int waveNumber = 0;
        int playerCombo = 0;
        bool topLevelShieldHit = false;

        SDL_Color white = { 255, 255, 255 };

        Texture logoTexture;
        logoTexture.setup(454*SCALE_FACTOR,92*SCALE_FACTOR,gameRenderer);
        logoTexture.loadFromFile("logo.png");

        TTF_Font* Sans = TTF_OpenFont("resources/sans.ttf",25);

        Texture startGameText(gameRenderer);
        if(controller == nullptr) {
            startGameText.loadFromRenderedText("Press Enter to Start.", white, Sans);
        } else {
            startGameText.loadFromRenderedText("Press A to Start.", white, Sans);
        }
        startGameText.render((WINDOW_WIDTH-startGameText.getWidth())/2,300*SCALE_FACTOR);

        Texture waveNumberText(gameRenderer);
        Texture comboNumberText(gameRenderer);
        Texture fpsText(gameRenderer);

        float lastFPS = 0;

        //Game Loop
        while(!quit) {

            while(!started && !quit) {
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
                    }
                }

                logoTexture.render(173*SCALE_FACTOR,100*SCALE_FACTOR);
                SDL_SetRenderDrawColor(gameRenderer, 105, 105, 105, 255);
                SDL_RenderPresent(gameRenderer);
            }

            inWave = true;
            waveNumber++;
            std::vector<Entity> eRobots = getWaveEnemyEntities(waveNumber,1, &enemySpawns);
            std::vector<Robor> robors;
            std::vector<Entity*> allCharacterEntities;
            allCharacterEntities.push_back(timpy.getEntity());

            for (auto it = eRobots.begin(); it != eRobots.end(); ++it) {
                robors.emplace_back(&(*it));
                allCharacterEntities.push_back(&(*it));
            }
            timpy.getEntity()->spawn();
            waveNumberText.loadFromRenderedText("Wave: " + std::to_string(waveNumber), white, Sans);
            comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);

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
                        if(e.key.keysym.sym == SDLK_SPACE) {
                            if(timpy.getDirection() && canShoot && timpy.getWeapon() == Weapon::revolver && shootingReset) {
                                eBullets.emplace_back(timpy.getEntity()->getRect().x+60*SCALE_FACTOR,timpy.getEntity()->getRect().y+19*SCALE_FACTOR,1000*SCALE_FACTOR,0,gameRenderer);
                                bullets.emplace_back(&eBullets.back());
                                bullets.back().setIterator(--eBullets.end());
                                canShoot = false;
                                shootingReset = false;
                            } else if(canShoot && timpy.getWeapon() == Weapon::revolver && shootingReset) {
                                eBullets.emplace_back(timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y+19*SCALE_FACTOR,-1000*SCALE_FACTOR,0,gameRenderer);
                                eBullets.emplace_back(timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y+19*SCALE_FACTOR,-1000*SCALE_FACTOR,0,gameRenderer);
                                bullets.emplace_back(&eBullets.back());
                                bullets.back().setIterator(--eBullets.end());
                                canShoot = false;
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
                    } else if( e.type == SDL_JOYAXISMOTION ) {
                        if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                            if(timpy.getDirection() && canShoot && timpy.getWeapon() == Weapon::revolver && shootingReset) {
                                eBullets.emplace_back(timpy.getEntity()->getRect().x+60*SCALE_FACTOR,timpy.getEntity()->getRect().y+19*SCALE_FACTOR,1000*SCALE_FACTOR,0,gameRenderer);
                                bullets.emplace_back(&eBullets.back());
                                bullets.back().setIterator(--eBullets.end());
                                canShoot = false;
                                shootingReset = false;
                            } else if(canShoot && timpy.getWeapon() == Weapon::revolver && shootingReset) {
                                eBullets.emplace_back(timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y+19*SCALE_FACTOR,-1000*SCALE_FACTOR,0,gameRenderer);
                                bullets.emplace_back(&eBullets.back());
                                bullets.back().setIterator(--eBullets.end());
                                canShoot = false;
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

                    }
                }

                Uint32 current = SDL_GetTicks();
                float dt = (current - lastUpdate) / 1000.0f;
                lastUpdate = current;


                if(developerMode) {
                    if(bullets.empty()) {
                        canShoot = true;
                    }
                } else {
                    if (lastShotTimeDifference > 0.75) {
                        lastShotTimeDifference = 0;
                        canShoot = true;
                    } else if(!canShoot) {
                        lastShotTimeDifference += dt;
                        timeToShoot.w = 75*lastShotTimeDifference*SCALE_FACTOR*1.333;
                    }
                }




                SDL_RenderClear(gameRenderer);

                bool robotAlive = false;
                bool playerAlive = true;

                //Render/Move Bullets
                for (auto it = bullets.begin(); it != bullets.end(); ++it) {
                    it->render();
                    if(it->move(dt, platforms, developerMode)) {
                        eBullets.erase(it->getIterator());
                        bullets.erase(it);
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

                        if(!firstLoop) {
                            it->move(dt, platforms);
                        }
                        it->render();
                        if(timpy.getWeapon() == Weapon::knife && Entity::isColliding(it->getEntity()->getRect(),timpy.getWeaponRect())) {
                            it->alive = false;
                        } else {
                            if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                if(timpy.getShield() == 2) {
                                    topLevelShieldHit = true;
                                    timpy.decreaseShield();
                                    timpy.getEntity()->despawn();
                                    timpy.getEntity()->spawn();
                                    playerCombo = 0;
                                    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);
                                } else if(timpy.getShield() == 1) {
                                    timpy.decreaseShield();
                                    timpy.getEntity()->despawn();
                                    timpy.getEntity()->spawn();
                                    playerCombo = 0;
                                    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);
                                } else {
                                    if(timpy.getHP() == 2) {
                                        timpy.damage();
                                        timpy.getEntity()->despawn();
                                        timpy.getEntity()->spawn();
                                        playerCombo = 0;
                                        comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);
                                    } else if (timpy.getHP() == 1) {
                                        playerAlive = false;
                                        waveNumber = 0;
                                        timpy.setHP(2);
                                        timpy.getEntity()->despawn();
                                        playerCombo = 0;
                                        comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);
                                    }
                                }
                            }
                        }
                        for(auto bit = bullets.begin(); bit != bullets.end(); ++bit) {
                            if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                it->alive = false;
                                eBullets.erase(bit->getIterator());
                                bullets.erase(bit);
                                playerCombo++;

                            }
                        }
                        comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, Sans);
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
                    timpy.move(dt, platforms);
                    timpy.render();
                }

                SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                if(developerMode) {
                    SDL_RenderDrawRect(gameRenderer,&timpy.getEntity()->getRect());
                }

                renderPlatforms(platforms);

                checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                waveNumberText.render(10,10);
                comboNumberText.render(10,50);

                if(developerMode) {
                    fpsText.loadFromRenderedText("FPS: " + std::to_string(lastFPS), white, Sans);
                    fpsText.render(10,90);
                }


                switch(timpy.getShield()) {
                    case 2: {
                        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield1);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield2);
                    } break;
                    case 1: {
                        SDL_SetRenderDrawColor(gameRenderer, 183, 201, 226, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield2);
                        SDL_SetRenderDrawColor(gameRenderer, 0, 0, 255, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield1);
                    } break;
                    default: {
                        SDL_SetRenderDrawColor(gameRenderer, 183, 201, 226, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield1);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerShield2);
                    } break;
                }

                switch(timpy.getHP()) {
                    case 2: {
                        SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerHealth1);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerHealth2);
                    } break;
                    case 1: {
                        SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerHealth1);
                        SDL_SetRenderDrawColor(gameRenderer, 170, 104, 95, 255);
                        SDL_RenderFillRect(gameRenderer,&timpy.playerHealth2);
                    } break;
                    default:
                        break;
                }

                if(playerCombo == 5 && (timpy.getShield() == 0 || topLevelShieldHit)) {
                    timpy.increaseShield();
                    topLevelShieldHit = false;
                } else if(playerCombo == 10 && timpy.getShield() == 1) {
                    timpy.increaseShield();
                }


                SDL_SetRenderDrawColor(gameRenderer, 150, 150, 150, 255);
                SDL_RenderFillRect(gameRenderer,&timeToShootBack);

                SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
                SDL_RenderFillRect(gameRenderer,&timeToShoot);

                SDL_SetRenderDrawColor(gameRenderer, 16, 16, 16, 255);
                SDL_RenderPresent(gameRenderer);

                Uint64 end = SDL_GetPerformanceCounter();

                float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
                lastFPS = (1.0f / elapsed);
            }
        }

        TTF_CloseFont(Sans);

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

void loadLevelFromCSV(std::string filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns) {
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
            platforms.emplace_back(std::stoi(data[i][1]),std::stoi(data[i][2])+25,gameRenderer);
        }
        if(std::stoi(data[i][0]) == 1) {
            playerSpawns.emplace_back(std::stoi(data[i][1]),std::stoi(data[i][2])+25,54,54);
        }
        if(std::stoi(data[i][0]) == 2) {
            enemySpawns.emplace_back(std::stoi(data[i][1]),std::stoi(data[i][2])+25,54,54);
        }
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

        //TODO: Test to see if a controller has been connected and allow the user to use that after the game has been launched.
        controller = SDL_GameControllerOpen(0);
        if(controller == nullptr) {
            SDL_Log( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
        } else {
            SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
        }

        char appDataPath[MAX_PATH];
        if (SHGetFolderPathA(nullptr, CSIDL_APPDATA , nullptr, 0, appDataPath) != S_OK) {
            SDL_Log("Failed to get AppData path");
            success = false;
        }

        // Append the new folder name to the AppData path
        std::string folderName = "OverGravity";
        gameFilesPath =  std::string(appDataPath) + "\\" + folderName;

        //TODO: Don't overwrite level file that is already there
        //TODO: Add ability to load constants from file
        if (CreateDirectoryA(gameFilesPath.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
            std::string destinationFile = std::string(gameFilesPath) + "\\level1.csv";
            if (CopyFileA("resources/levels/level1.csv", destinationFile.c_str(), FALSE)) {
                SDL_Log("Copied level1.csv");
            } else {
                SDL_Log("Failed to copy level files: ", GetLastError());
            }
        } else {
            SDL_Log("Failed to create folder. Error: ", SDL_GetError());
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
