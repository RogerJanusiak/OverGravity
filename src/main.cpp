#include <chrono>

#include "../includes/Animations.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>

#include "../includes/Bullet.h"
#include "../includes/Robor.h"
#include "../includes/GlobalConstants.h"
#include "../includes/Platform.h"
#include "../includes/Player.h"
#include "../includes/Spawn.h"
#include "../includes/State.h"

#include <SDL_ttf.h>
#include <thread>

#include "../includes/Roborto.h"
#include "../includes/Sound.h"
#include "../includes/UI.h"

SDL_Window *gameWindow = nullptr;
SDL_Renderer *gameRenderer = nullptr;

std::string gameFilesPath;

//Game Controller 1 handler
SDL_GameController* controller = nullptr;

bool init();
void close();
void checkIfSpawnsOccupied(std::vector<Spawn*>& allSpawns, std::list<Entity*>& allCharacterEntities);
void renderPlatforms(std::list<Platform*>& platforms);
std::list<Entity> getWaveEnemyEntities(int waveNumber,int divisor, int notDiv, std::vector<Spawn>* spawns);
void loadLevelFromCSV(std::string& filePath, std::list<Platform>& platforms, std::vector<Spawn>& enemySpawns, std::vector<Spawn>& playerSpawns);
void loadController();

void checkIfSpawnsAreOnScreen(std::vector<Spawn>& enemySpawns);

void moveCamera(std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns, float dt);

bool pauseEnemy = false;

Player* timpyPointer = nullptr;
const int timpyXVelocity = scale(350);

SDL_Rect fullScreenRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

State state;

void startScreen() {
    SDL_RenderClear(gameRenderer);

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            state.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE && state.menu == level) {
                state.menu = head;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(state,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                controllerEvent(state,MENU_CONTROL::back);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(state,MENU_CONTROL::connect);
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(state,MENU_CONTROL::disconnect);
            controller = nullptr;
            state.controller = false;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(state);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(state);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::down);
                    state.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::up);
                    state.controllerStickReset = false;
                }
            } else {
                state.controllerStickReset = true;
            }
        }
    }

    renderMenu(state);
    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
    SDL_RenderPresent(gameRenderer);
}

void pauseScreen() {
    SDL_RenderClear(gameRenderer);

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            state.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                state.menu = notInMenu;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(state,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                state.menu = notInMenu;
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(state,MENU_CONTROL::connect);
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(state,MENU_CONTROL::disconnect);
            controller = nullptr;
            state.controller = false;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(state);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(state);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::down);
                    state.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(state.controllerStickReset) {
                    controllerEvent(state,MENU_CONTROL::up);
                    state.controllerStickReset = false;
                }
            } else {
                state.controllerStickReset = true;
            }
        }
    }

    renderMenu(state);
    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
    SDL_RenderPresent(gameRenderer);
}

int main( int argc, char* args[] ) {
    if(!init()) {
        SDL_Log("Initialization failed!\n");
    } else {

        SDL_Event e;
        Uint32 lastUpdate = SDL_GetTicks();

        UI_init(gameRenderer, state);

        Sound explosion("resources/sounds/shortExplosion.wav", 0,-1);
        Sound mediumExplosion("resources/sounds/mediumExplosion.wav", 0,-1);
        Sound song("resources/sounds/song.wav", -1,0);

        std::list<Explosion> explosions;

        song.play();

        if(controller != nullptr) {
            controllerEvent(state,MENU_CONTROL::connect);
        }

        //Game Loop
        while(!state.quit) {

            if(controller != nullptr) {
                controllerEvent(state,MENU_CONTROL::connect);
            }

            while(!state.started && !state.quit) {
                startScreen();
            }

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

            Weapon revolver(Weapon_Type::revolver,gameRenderer);
            Weapon knife(Weapon_Type::knife,gameRenderer);
            Weapon laserPistol(Weapon_Type::laserPistol,gameRenderer);

            Entity eTimpy = Entity(&playerSpawns,gameRenderer);
            Player timpy = Player(&eTimpy,&revolver);
            timpyPointer = &timpy;

            state.camY = 0;
            state.camV = 0;

            bool waveOverride = false;

            bool shootingReset = true;
            bool c4Exploded = false;

            bool inWave;
            int waveNumber = 0;

            float lastFPS = 0;

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

            while(state.started && !state.quit) {

                bool leftMovement = false;
                bool rightMovement = false;

                if(waveNumber == 0) {
                    timpy.setSecondaryWeapon(nullptr);
                    timpy.setAbility(none);
                }
                inWave = true;
                waveNumber++;

                std::list<Entity> tempRobors = getWaveEnemyEntities(waveNumber,1,3, &enemySpawns);
                std::list<Entity> eRobots;
                std::list<Robor> robors;

                std::list<Entity> tempRobortos = getWaveEnemyEntities(waveNumber,3,-1, &enemySpawns);
                std::list<Entity> eRobortos;
                std::list<Roborto> robortos;

                std::list<Entity*> allCharacterEntities;

                allCharacterEntities.push_back(timpy.getEntity());

                for (auto it = tempRobors.begin(); it != tempRobors.end(); ++it) {
                    eRobots.push_back(*it);
                    robors.emplace_back(&(*it));
                    allCharacterEntities.emplace_back(&(*it));
                }

                for (auto it = tempRobortos.begin(); it != tempRobortos.end(); ++it) {
                    eRobortos.push_back(*it);
                    robortos.emplace_back(&(*it));
                    allCharacterEntities.emplace_back(&(*it));
                }

                checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                state.menu = upgrade;
                loadUpgradeMenu(state);
                while((waveNumber) % 1 == 0 && state.menu == upgrade && !state.quit && waveNumber-1 != 0) {
                    while(SDL_PollEvent(&e) != 0) {
                        if( e.type == SDL_QUIT ) {
                            state.quit = true;
                        } else if(e.type == SDL_KEYDOWN) {
                            if(e.key.keysym.sym == SDLK_ESCAPE) {
                                state.menu = notInMenu;
                            }
                        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
                            menuSelect(state);
                        } else if(e.type == SDL_MOUSEMOTION) {
                            mouseMove(state);
                        } else if( e.type == SDL_JOYAXISMOTION) {
                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) {
                                controllerEvent(state,MENU_CONTROL::right);
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) {
                                controllerEvent(state,MENU_CONTROL::left);
                            } else if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                                controllerEvent(state,MENU_CONTROL::down);
                            } else if (SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                                controllerEvent(state,MENU_CONTROL::up);
                            }
                        } else if( e.type == SDL_JOYBUTTONDOWN ) {
                            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {

                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                                state.menu = notInMenu;
                            }
                        }
                    }

                    SDL_RenderClear(gameRenderer);

                    for(auto spawn : allSpawns) {
                        spawn->render(gameRenderer);
                    }

                    for (auto it = bullets.begin(); it != bullets.end();++it) {
                        it->render();
                    }

                    for (auto it = robors.begin(); it != robors.end();++it) {
                        it->render();
                    }

                    for (auto it = robortos.begin(); it != robortos.end();++it) {
                        it->render();
                    }

                    renderPlatforms(platforms);

                    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 200);
                    SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
                    SDL_RenderFillRect(gameRenderer, &fullScreenRect);

                    renderMenu(state);

                    SDL_SetRenderDrawColor(gameRenderer, 26, 26, 26, 255);
                    SDL_RenderPresent(gameRenderer);

                }
                switch(state.weapon1) {
                    case 0:
                        timpy.setPrimaryWeapon(&revolver);
                        break;
                    case 1:
                        timpy.setPrimaryWeapon(&laserPistol);
                        break;
                    case 2:
                        timpy.setPrimaryWeapon(&knife);
                        break;
                    default:
                        timpy.setPrimaryWeapon(nullptr);
                }
                switch(state.weapon2) {
                    case 0:
                        timpy.setSecondaryWeapon(&revolver);
                        break;
                    case 1:
                        timpy.setSecondaryWeapon(&laserPistol);
                        break;
                    case 2:
                        timpy.setSecondaryWeapon(&knife);
                        break;
                    default:
                        timpy.setSecondaryWeapon(nullptr);
                }
                state.menu = notInMenu;

                bool waveStarted = false;
                Uint32 startWaveLoad = SDL_GetTicks();

                while(state.started && inWave && !state.quit) {
                    Uint64 start = SDL_GetPerformanceCounter();

                    //Controls Loop
                    while(SDL_PollEvent(&e) != 0 && state.menu != upgrade) {
                        if( e.type == SDL_QUIT ) {
                            state.quit = true;
                        } else if( e.type == SDL_KEYDOWN ) {
                            if(e.key.keysym.sym == SDLK_1) {
                                state.developerMode = !state.developerMode;
                            }
                            if(e.key.keysym.sym == SDLK_2 && state.developerMode) {
                                waveNumber = 30;
                                waveOverride = true;
                            }
                            if(e.key.keysym.sym == SDLK_3 && state.developerMode) {
                                timpy.getEntity()->forceSpawn();
                            }
                            if(e.key.keysym.sym == SDLK_4 && state.developerMode) {
                                pauseEnemy = !pauseEnemy;
                            } else if(e.key.keysym.sym == SDLK_ESCAPE) {
                                state.menu = pause;
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
                            } else if(e.key.keysym.sym == SDLK_r) {
                                timpy.getWeapon()->forceReload();
                            }
                            if(e.key.keysym.sym == SDLK_SPACE && waveStarted) {
                                if(shootingReset) {
                                    timpy.getWeapon()->shoot(&eBullets,&bullets,state,timpy.getDirection(),timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y);
                                    shootingReset = false;
                                }
                            } else if(e.key.keysym.sym == SDLK_e) {
                                switch(timpy.useAbility()) {
                                    case respawn: {
                                        timpy.getEntity()->forceSpawn();
                                    } break;
                                    case c4: {
                                        mediumExplosion.play();
                                        c4Exploded = true;
                                    } break;
                                    default:
                                        break;
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
                        } else if( e.type == SDL_JOYAXISMOTION) {
                            if(SDL_GameControllerGetAxis(controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                                if(shootingReset) {
                                    if(timpy.getWeapon()->shoot(&eBullets,&bullets,state,timpy.getDirection(),timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y)) {
                                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 150 );
                                    } else {
                                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 75 );
                                    }
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
                        } else if( e.type == SDL_JOYBUTTONDOWN ) {
                            if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y) == 1) {
                                timpy.changeWeapon();
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                                switch(timpy.useAbility()) {
                                    case respawn: {
                                        timpy.getEntity()->forceSpawn();
                                    } break;
                                    case c4: {
                                        mediumExplosion.play();
                                        c4Exploded = true;
                                    } break;
                                    default:
                                        break;
                                }
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                                state.menu = pause;
                            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
                                timpy.getWeapon()->forceReload();
                            }
                        }
                    }

                    Uint32 current = SDL_GetTicks();
                    float dt = (current - lastUpdate) / 1000.0f;
                    lastUpdate = current;

                    if((current-startWaveLoad)/1000.0f > 1) {
                        waveStarted = true;
                    }

                    if(timpy.getWeapon()->isReloadable()) {
                        updateTimeToShoot(scale(timpy.getWeapon()->reload(dt)));
                    } else {
                        updateTimeToShoot(scale(75));
                    }

                    updateTimeToAbility(scale(timpy.charge(dt)));
                    if(timpy.getWeapon()->wasJustReloaded()) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 1 / 2, 0xFFFF * 1 / 2, 50 );
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
                    int enemiesAlive = 0;
                    for (auto it = robors.begin(); it != robors.end();++it) {
                        bool firstLoop = false;
                        if(!it->getEntity()->isSpawned()) {
                            it->getEntity()->spawn(state.enemiesAlive <= 5);
                            firstLoop = true;
                        }
                        if(it->alive && it->getEntity()->isSpawned()) {

                            if(!firstLoop && waveStarted && !pauseEnemy && state.menu != upgrade) {
                                it->move(dt, platforms,state.camY,state.levelHeight);
                            }
                            it->render();
                            if(timpy.getWeapon()->getType() == Weapon_Type::knife && Entity::isColliding(it->getEntity()->getRect(),timpy.getWeaponRect())) {
                                it->alive = false;
                                explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                timpy.increaseCombo();
                                explosion.play();
                            } else {
                                if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                    if(timpy.getEntity()->getRect().y + (timpy.getEntity()->getRect().h-it->getEntity()->getRect().h) < it->getEntity()->getRect().y
                                        && timpy.getAbility() == Ability::bounce) {
                                        timpy.getEntity()->setYVelocity(-1800);
                                        timpy.increaseCombo();
                                        explosion.play();
                                        } else {
                                            if(timpy.damage()) {
                                                playerAlive = false;
                                                waveNumber = 0;
                                                timpy.zeroCombo();
                                                updateInGameText(timpy.getCombo(),waveNumber, timpy.getAbility());
                                            }
                                            playerDamaged = true;
                                        }
                                    it->alive = false;
                                    explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                }
                            }
                            for(auto bit = bullets.begin(); bit != bullets.end();) {
                                if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                    eBullets.erase(bit->getIterator());
                                    bit = bullets.erase(bit);
                                    timpy.increaseCombo();
                                    it->alive = false;
                                    explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                    explosion.play();
                                } else {
                                    ++bit;
                                }
                            }
                            updateInGameText(timpy.getCombo(),waveNumber, timpy.getAbility());
                            if(state.developerMode) {
                                SDL_RenderDrawRect(gameRenderer,&it->getEntity()->getRect());
                            }
                            if(c4Exploded) {
                                int c4x = timpy.getC4Entity()->getRect().x;
                                int c4y = timpy.getC4Entity()->getRect().y;
                                if(pow(pow(c4x - it->getEntity()->getRect().x,2)+pow(c4y - it->getEntity()->getRect().y,2),0.5) < scale(200)) {
                                    it->alive = false;
                                    explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                    timpy.increaseCombo();
                                }
                            }
                        }
                        if(it->alive) {
                            robotAlive = true;
                            enemiesAlive++;
                        }
                    }

                    for (auto it = robortos.begin(); it != robortos.end();++it) {
                        bool firstLoop = false;
                        if(!it->getEntity()->isSpawned()) {
                            it->getEntity()->spawn(state.enemiesAlive <= 5);
                            firstLoop = true;
                        }
                        if(it->alive && it->getEntity()->isSpawned()) {
                            if(!firstLoop && waveStarted && !pauseEnemy) {
                                it->move(dt, platforms,state);
                            }
                            it->render();
                            if( Entity::isColliding(it->getEntity()->getRect(),timpy.getEntity()->getRect())) {
                                if(timpy.getEntity()->getRect().y + (timpy.getEntity()->getRect().h-it->getEntity()->getRect().h) < it->getEntity()->getRect().y && timpy.getAbility() == Ability::bounce) {
                                    timpy.getEntity()->setYVelocity(-1800);
                                    explosion.play();
                                } else {
                                    if(timpy.damage()) {
                                        playerAlive = false;
                                        waveNumber = 0;
                                        timpy.zeroCombo();
                                        updateInGameText(timpy.getCombo(),waveNumber, timpy.getAbility());
                                    }
                                    playerDamaged = true;
                                }
                                it->alive = false;
                                explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                            }
                            for(auto bit = bullets.begin(); bit != bullets.end();) {
                                if(Entity::isColliding(it->getEntity()->getRect(),bit->getEntity()->getRect())) {
                                    eBullets.erase(bit->getIterator());
                                    bit = bullets.erase(bit);
                                    timpy.increaseCombo();
                                    it->alive = false;
                                    explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                    explosion.play();
                                } else {
                                    ++bit;
                                }
                            }
                            updateInGameText(timpy.getCombo(),waveNumber, timpy.getAbility());
                            if(state.developerMode) {
                                SDL_RenderDrawRect(gameRenderer,&it->getEntity()->getRect());
                            }
                            if(c4Exploded) {
                                int c4x = timpy.getC4Entity()->getRect().x;
                                int c4y = timpy.getC4Entity()->getRect().y;
                                if(pow(pow(c4x - it->getEntity()->getRect().x,2)+pow(c4y - it->getEntity()->getRect().y,2),0.5) < scale(200)) {
                                    it->alive = false;
                                    explosions.emplace_back(it->getEntity()->getRect().x+it->getEntity()->getRect().w/2,it->getEntity()->getRect().y+it->getEntity()->getRect().h/2,gameRenderer);
                                    timpy.increaseCombo();
                                }
                            }
                        }
                        if(it->alive) {
                            robotAlive = true;
                            enemiesAlive++;
                        }
                    }

                    state.enemiesAlive = enemiesAlive;

                    for(auto it = explosions.begin();it != explosions.end();) {
                        if(it->finished()) {
                            it = explosions.erase(it);
                        } else {
                            it->render();
                            it->advance();
                            ++it;
                        }
                    }


                    c4Exploded = false;

                    if(playerDamaged) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                        timpy.zeroCombo();
                        updateInGameText(timpy.getCombo(),waveNumber, timpy.getAbility());
                        timpy.charge(dt);
                        timpy.getEntity()->forceSpawn();
                    }

                    if(!playerAlive || waveOverride) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 750 );
                        inWave = false;
                        waveOverride = false;
                    } else {
                        inWave = robotAlive;
                    }

                    if(timpy.getEntity()->isSpawned()) {
                        if(waveStarted) {
                            timpy.move(dt, platforms,state.camY);
                        }
                        timpy.render();
                        state.playerX = timpy.getEntity()->getRect().x;
                        state.playerTileX = state.playerX/TILE_SIZE_SCALED+1;
                        state.playerTileY = (timpy.getEntity()->getRect().y-state.camY)/TILE_SIZE_SCALED;

                        if(state.developerMode) {
                            SDL_Rect playerTile = {(state.playerTileX-1)*TILE_SIZE_SCALED, state.playerTileY*TILE_SIZE_SCALED+state.camY,TILE_SIZE_SCALED,TILE_SIZE_SCALED};
                            SDL_SetRenderDrawColor(gameRenderer, 225, 225, 0, 255);
                            SDL_RenderDrawRect(gameRenderer, &playerTile);
                        }
                    }

                    if((timpy.getEntity()->getRect().y >= scale(195) && state.camY > -1*(scale(state.levelHeight)-WINDOW_HEIGHT)) || (timpy.getEntity()->getRect().y < 0 && state.camY < 0)) {
                        moveCamera(allCharacterEntities,platforms,allSpawns, dt);
                    }

                    SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
                    if(state.developerMode) {
                        SDL_RenderDrawRect(gameRenderer,&timpy.getEntity()->getRect());
                        SDL_RenderDrawRect(gameRenderer, timpy.getWheelRect());
                    }

                    checkIfSpawnsAreOnScreen(enemySpawns);

                    checkIfSpawnsOccupied(allSpawns,allCharacterEntities);

                    renderPlatforms(platforms);

                    renderInGameText(state.developerMode, lastFPS, waveStarted);

                    renderPlayerUI(&timpy);

                    SDL_SetRenderDrawColor(gameRenderer, 16, 16, 16, 255);
                    SDL_RenderPresent(gameRenderer);

                    Uint64 end = SDL_GetPerformanceCounter();

                    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
                    lastFPS = (1.0f / elapsed);

                    if(controller != nullptr && state.menu == pause) {
                        controllerEvent(state,MENU_CONTROL::connect);
                    }

                    while(state.menu == pause && !state.quit && state.started) {
                        pauseScreen();
                        Uint32 current = SDL_GetTicks();
                        lastUpdate = current;
                    }

                }
            }
        }

    }
    close();
    return 0;
}

void checkIfSpawnsAreOnScreen(std::vector<Spawn>& enemySpawns) {
    for (auto sit = enemySpawns.begin(); sit != enemySpawns.end(); ++sit) {
        if(sit->getRect().x > 0 && sit->getRect().x < WINDOW_WIDTH
            && sit->getRect().y >= 0 && sit->getRect().y+sit->getRect().h <= WINDOW_HEIGHT) {
            sit->setOnScreen(true);
        } else {
            sit->setOnScreen(false);
        }
    }
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

void moveCamera(std::list<Entity*>& allCharacterEntities, std::list<Platform*>& platforms, std::vector<Spawn*>& allSpawns, float dt) {
    int vector = (scale(195)-timpyPointer->getEntity()->getRect().y)*scale(2)*dt;

    if(state.camY+vector > 0) {
        vector = state.camY*-1;
    } else if(state.camY+vector < -1*(scale(state.levelHeight)-WINDOW_HEIGHT)) {
        vector = -1*(scale(state.levelHeight)-WINDOW_HEIGHT)-state.camY;
    }

    state.camY += vector;

    timpyPointer->getC4Entity()->setPosition(timpyPointer->getC4Entity()->getRect().x, timpyPointer->getC4Entity()->getRect().y+vector);

    for (auto entites : allCharacterEntities) {
        entites->setPosition(entites->getRect().x,entites->getRect().y+vector);
    }
    timpyPointer->updateWheelRect();
    for (auto platform : platforms) {
        platform->setPosition(platform->getPlatformRect().x,platform->getPlatformRect().y+vector);
    }
    for(auto spawn : allSpawns) {
        spawn->setPosition(spawn->getRect().x,spawn->getRect().y+vector);
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

std::list<Entity> getWaveEnemyEntities(const int waveNumber,const int divisor,int notDiv, std::vector<Spawn>* spawns) {
    std::list<Entity> entities;
    for(int i = 1; i <= waveNumber; i++) {
        if(notDiv != -1) {
            if(i % divisor == 0 && i % notDiv != 0) {
                entities.emplace_back(spawns,gameRenderer);
            }
        } else {
            if(i % divisor == 0) {
                entities.emplace_back(spawns,gameRenderer);
            }
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
    state.levelHeight = row*TILE_SIZE;

    if(!state.levelMap.empty()) {
        state.levelMap.clear();
    }

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
        state.levelMap.push_back(mapRow);
    }
}

void loadController() {
    controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
    if(controller == nullptr) {
        state.controller = false;
    } else {
        SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
        state.controller = true;
    }
}

bool init() {
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO ) < 0 ) {
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

    Mix_CloseAudio();

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
