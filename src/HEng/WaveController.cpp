#include <memory>
#include "../../includes/HEng/WaveController.h"
#include "../../includes/Utils/Input.h"

WaveController::WaveController(GlobalGameState& ggs, Run& run) : ggs(ggs), run(run), player(run.getPlayer()) {
    timeToShootBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShootBack.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShootBack.w = scalePlayerUI(75);
    timeToShootBack.h = scalePlayerUI(15);

    timeToShoot.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToShoot.y = WINDOW_HEIGHT-scalePlayerUI(50);
    timeToShoot.w = scalePlayerUI(75);
    timeToShoot.h = scalePlayerUI(15);

    timeToAbilityBack.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbilityBack.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbilityBack.w = scalePlayerUI(75);
    timeToAbilityBack.h = scalePlayerUI(15);

    timeToAbility.x = WINDOW_WIDTH-scalePlayerUI(90);
    timeToAbility.y = WINDOW_HEIGHT-scalePlayerUI(75);
    timeToAbility.w = scalePlayerUI(75);
    timeToAbility.h = scalePlayerUI(15);

    shieldBackRect.x = scalePlayerUI(10);
    shieldBackRect.y = WINDOW_HEIGHT-scalePlayerUI(40);
    shieldBackRect.w = scalePlayerUI(75);
    shieldBackRect.h = scalePlayerUI(15);

    shieldRect.x = scalePlayerUI(10);
    shieldRect.y = WINDOW_HEIGHT-scalePlayerUI(40);
    shieldRect.w = scalePlayerUI(75);
    shieldRect.h = scalePlayerUI(15);

    healthBackRect.x = scalePlayerUI(10);
    healthBackRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthBackRect.w = scalePlayerUI(75);
    healthBackRect.h = scalePlayerUI(15);

    healthRect.x = scalePlayerUI(10);
    healthRect.y = WINDOW_HEIGHT-scalePlayerUI(20);
    healthRect.w = scalePlayerUI(75);
    healthRect.h = scalePlayerUI(15);

    healthText.setup(ggs.renderer);
    healthText.loadFromRenderedText("200", ggs.black,ggs.verySmall);

    shieldText.setup(ggs.renderer);
    shieldText.loadFromRenderedText("0", ggs.black,ggs.verySmall);

    waveNumberText.setup(ggs.renderer);
    waveNumberTitle.setup(ggs.renderer);
    comboNumberText.setup(ggs.renderer);
    playerXPText.setup(ggs.renderer);
    fpsText.setup(ggs.renderer);

    startWave();
    startLevel();
    updateWaveText();
}

void WaveController::operate() {
	if(currentWave != nullptr) {
		currentWave->render();
	    renderPlayerUI();
	    renderWaveText();
		if(!currentWave->runWave()) {
		    run.nextWave();
		    startWave();
		}
	}
    if(ggs.updateText) {
        updatePlayerUIText();
        ggs.updateText = false;
    }
    operatePlayer();
}

void WaveController::operatePlayer() {
    player.move(ggs, run.getLevel().getPlatforms(), run.getLevel().getTeleports());
    player.tickInvicibilty(ggs.dt);
    updateTimeToShoot(scalePlayerUI(player.getWeapon()->reload(ggs.dt)));
    player.render();
}

void WaveController::updateTimeToShoot(const double width) {
    timeToShoot.w = width;
}

void WaveController::startLevel() const {
    player.getEntity()->forceSpawn();
}

void WaveController::startWave() {
	inWave = true;
	currentWave = std::make_unique<Wave>(ggs,run.getPlayer(), run.getLevel(), run.getWaveNumber());
	player.getEntity()->setSpawns(run.getLevel().getPlayerSpawns());
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
                }
            } else if(e.key.keysym.sym == SDLK_e) {
                //player.useAbility(state);
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
                    if(player.getWeapon()->shoot(&eBullets,&bullets,state,player.getDirection(),player.getEntity()->getRect().x,player.getEntity()->getRect().y)) {
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
                player.changeWeapon();
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                player.useAbility(state);
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == 1) {
                state.menu = pause;
            } else if(SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X) == 1) {
                player.getWeapon()->forceReload();
            }*/
        }
    }
}

void WaveController::updatePlayerUIText() {
    updateWaveText();
    healthText.loadFromRenderedText(std::to_string(player.getHealth()), ggs.black, ggs.verySmall);
    shieldText.loadFromRenderedText(std::to_string(player.getShield()), ggs.black, ggs.verySmall);
}

void WaveController::renderPlayerUI() {
    int bulletsInClip = player.getWeapon()->getBulletsInClip();
    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShootBack);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&timeToShoot);

    if(player.getAbility() != none) {
        SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
        SDL_RenderFillRect(ggs.renderer,&timeToAbilityBack);
        SDL_SetRenderDrawColor(ggs.renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(ggs.renderer,&timeToAbility);
    }

    healthRect.w = scalePlayerUI(player.getHealthPercentage()*75);

    shieldRect.w = scalePlayerUI(player.getShieldPercentage()*75);

    SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(ggs.renderer,&healthBackRect);
    SDL_RenderFillRect(ggs.renderer,&shieldBackRect);

    SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(ggs.renderer,&healthRect);

    SDL_SetRenderDrawColor(ggs.renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(ggs.renderer,&shieldRect);

    healthText.render(scalePlayerUI(12),WINDOW_HEIGHT-scalePlayerUI(19));
    shieldText.render(scalePlayerUI(12),WINDOW_HEIGHT-scalePlayerUI(39));

    for(int i = 0; i < player.getWeapon()->getClipSize(); i++) {
        if(bulletsInClip>i) {
            SDL_SetRenderDrawColor(ggs.renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(ggs.renderer, 150, 150, 150, 255);
        }
        SDL_Rect tempRect;
        tempRect.x = WINDOW_WIDTH-scalePlayerUI(30)-scalePlayerUI(20*i);
        tempRect.y = WINDOW_HEIGHT-scalePlayerUI(25);
        tempRect.w = scalePlayerUI(15);
        tempRect.h = scalePlayerUI(15);
        SDL_RenderFillRect(ggs.renderer,&tempRect);
    }

}

void WaveController::updateWaveText() {
    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(player.getCombo()), ggs.white, ggs.buttonFont);
    waveNumberText.loadFromRenderedText("Wave: " + std::to_string(run.getWaveNumber()), ggs.white, ggs.buttonFont);
    waveNumberTitle.loadFromRenderedText("Wave " + std::to_string(run.getWaveNumber()) + " Start!", ggs.white, ggs.title);
    playerXPText.loadFromRenderedText("XP: " + std::to_string(player.getXP()), ggs.white, ggs.buttonFont);
}

void WaveController::renderWaveText() {
    waveNumberText.render(scaleUI(10),scaleUI(5));
    comboNumberText.render(scaleUI(10),scaleUI(30));
    playerXPText.render(scaleUI(10),scaleUI(55));
    if(ggs.developerMode) {
        fpsText.loadFromRenderedText("FPS: " + std::to_string(ggs.fps), ggs.white, ggs.buttonFont);
        fpsText.render(scaleUI(10),scaleUI(80));
    }
    if(!currentWave->hasWaveStarted()) {
        waveNumberTitle.render((WINDOW_WIDTH-waveNumberTitle.getWidth())/2,scaleUI(200));
    }
}

