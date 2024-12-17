#include <memory>
#include "../includes/WaveController.h"
#include "../includes/Input.h"

void WaveController::operate() const {
	if(currentWave != nullptr) {
		currentWave->render();
		currentWave->runWave();
	}
    operatePlayer();
}

void WaveController::operatePlayer() const {
    player.move(ggs, run.getLevel().getPlatforms(), run.getLevel().getTeleports());
    player.tickInvicibilty(ggs.dt);
    player.render();
}

void WaveController::startWave() {
	inWave = true;
	currentWave = std::make_unique<Wave>(ggs,run.getPlayer(), run.getLevel(), run.getWaveNumber());
	player.getEntity()->setSpawns(run.getLevel().getPlayerSpawns());
	player.getEntity()->forceSpawn();
}

void WaveController::readInput() {
	SDL_Event e;

    // TODO: Roborto pathing is having them go left out of a spawn at tileX = 0...
	while(SDL_PollEvent(&e) != 0) {
		if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_1) {
                ggs.developerMode = !ggs.developerMode;
            } else if(e.key.keysym.sym == SDLK_2 && ggs.developerMode) {
                ggs.pauseEnemies = !ggs.pauseEnemies;
            } else if(e.key.keysym.sym == SDLK_ESCAPE) {
                // TODO: Add pause men bu
            } else if(e.key.keysym.sym == SDLK_d) {
                player.setXNormalV(1);
                rightMovement = true;
            } else if(e.key.keysym.sym == SDLK_a) {
                player.setXNormalV(-1);
                leftMovement = true;
            } else if(e.key.keysym.sym == SDLK_l) {
                player.setDirection(true);
            } else if(e.key.keysym.sym == SDLK_j) {
                player.setDirection(false);
            } else if(e.key.keysym.sym == SDLK_w) {
                player.changeWeapon();
            } else if(e.key.keysym.sym == SDLK_r) {
                player.getWeapon()->forceReload();
            }
            // From Refactor: Had a state variable for resetShooting, not sure how it was used, removed it.
            if(e.key.keysym.sym == SDLK_SPACE) {
                if(shootingReset && currentWave != nullptr) {
                    player.getWeapon()->shoot(currentWave->getBulletEntities(),currentWave->getBullets(),player.getDirection(),player.getEntity()->getRect().x,player.getEntity()->getRect().y);
                    shootingReset = false;
                    SDL_Log("Test");
                }
            } else if(e.key.keysym.sym == SDLK_e) {
                //timpy.useAbility(state);
            }
        } else if(e.type == SDL_KEYUP) {
            if(e.key.keysym.sym == SDLK_d)
                rightMovement = false;
            if(e.key.keysym.sym == SDLK_a)
                leftMovement = false;
            if(!leftMovement && !rightMovement) {
                player.setXNormalV(0);
            } else if (leftMovement) {
                player.setXNormalV(-1);
            } else {
                player.setXNormalV(1);
            }

            if (e.key.keysym.sym == SDLK_SPACE) {
                shootingReset = true;
            }
        } else if( e.type == SDL_JOYAXISMOTION) {
            /*if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > JOYSTICK_DEAD_ZONE) {
                if(shootingReset && !state.teleportSelection && !state.c4Placed) {
                    if(timpy.getWeapon()->shoot(&eBullets,&bullets,state,timpy.getDirection(),timpy.getEntity()->getRect().x,timpy.getEntity()->getRect().y)) {
                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 150 );
                    } else {
                        SDL_GameControllerRumble( controller, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 75 );
                    }
                    shootingReset = false;
                }
            } else {
                shootingReset = true;
            }*/

            // TODO: Move wave input to an function that is called by an enum so that code doesn't have to be replcated. Look at the menu code for example.
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) > JOYSTICK_DEAD_ZONE) {
                player.setDirection(true);
            } else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) < -JOYSTICK_DEAD_ZONE) {
                player.setDirection(false);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            ggs.controller = nullptr;
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            /*if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y) == 1) {
                timpy.changeWeapon();
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                timpy.useAbility(state);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                state.menu = pause;
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
                timpy.getWeapon()->forceReload();
            }*/
        }
    }
}

