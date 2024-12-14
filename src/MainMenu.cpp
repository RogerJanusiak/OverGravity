#include <SDL_events.h>

#include "../includes/MainMenu.h"

#include <map>

#include "../includes/Input.h"

void handleMenuInput(GlobalGameState& ggs, MainMenuState& mms, UI_Menu* currentMenu);
void initMenus(GlobalGameState& ggs, UI_Menu& main, UI_Menu& levelSelect);

Sound buttonSound;
Texture logoTexture;

UI_Menu mainMenu(3);
UI_Menu levelSelect(4);

UI_Menu* currentMenu = &mainMenu;

void runMainMenu(GlobalGameState& ggs) {

    initMenus(ggs,mainMenu,levelSelect);

	bool levelSelected = false;
	while(!levelSelected && !ggs.quit) {

        if(ggs.mms.currentMenu == head) {
            currentMenu = &mainMenu;
        } else {
            currentMenu = &levelSelect;
        }


		SDL_RenderClear(ggs.renderer);

	    handleMenuInput(ggs,ggs.mms, currentMenu);

	    currentMenu->render();

        SDL_SetRenderDrawColor(ggs.renderer, 26, 26, 26, 255);
        SDL_RenderPresent(ggs.renderer);
	}
}

void handleMenuInput(GlobalGameState& ggs, MainMenuState& mms, UI_Menu* currentMenu) {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
        if( e.type == SDL_QUIT ) {
            ggs.quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            if(e.key.keysym.sym == SDLK_ESCAPE && mms.currentMenu == level) {
                mms.currentMenu = head;
            }
        } else if( e.type == SDL_JOYBUTTONDOWN ) {
            if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::select);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B) == 1) {
                if(ggs.mms.currentMenu == level) {
                    ggs.mms.currentMenu = head;
                    currentMenu = &mainMenu;
                    mainMenu.loadMenu();
                }
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::up);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::down);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::left);
            } else if(SDL_GameControllerGetButton(ggs.controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1) {
                controllerEvent(*currentMenu,MENU_CONTROL::right);
            }
        } else if(e.type == SDL_JOYDEVICEADDED ) {
            controllerEvent(*currentMenu,MENU_CONTROL::connect);
            ggs.controller = loadController();
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            controllerEvent(*currentMenu,MENU_CONTROL::disconnect);
            ggs.controller = nullptr;
        } else  if( e.type == SDL_MOUSEMOTION) {
            mouseMove(ggs, *currentMenu);
        } else if(e.type == SDL_MOUSEBUTTONDOWN) {
            menuSelect(*currentMenu);
        } else if( e.type == SDL_JOYAXISMOTION) {
            if(SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                    controllerEvent(*currentMenu,MENU_CONTROL::down);
                    ggs.controllerStickReset = false;
                }
            } else if (SDL_GameControllerGetAxis(ggs.controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) {
                if(ggs.controllerStickReset) {
                    controllerEvent(*currentMenu,MENU_CONTROL::up);
                    ggs.controllerStickReset = false;
                }
            } else {
                ggs.controllerStickReset = true;
            }
        }
    }
}

void showLevelSelect(GlobalGameState& gss, int attr1, int attr2) {
    gss.mms.currentMenu = level;
    levelSelect.loadMenu();
}

void quitToDesktop(GlobalGameState& ggs, int attr1, int attr2) {
    ggs.quit = true;
}

void noAction(GlobalGameState& ggs, int attr1, int attr2) {}

void initMenus(GlobalGameState& ggs, UI_Menu& main, UI_Menu& levelSelect) {
    const int centeredX = (WINDOW_WIDTH-UI_Button::width)/2;

    buttonSound.init("resources/sounds/buttonClick.wav", 0,-1);

    main.setup(ggs.renderer, &buttonSound);
    const int arcadeModeButton = main.addButton(centeredX,scaleUI(215),"Arcade Mode",&ggs.white, ggs.buttonFont,-1,-1,-1,-1,&showLevelSelect,ggs);
    const int settingsButton = main.addButton(centeredX,scaleUI(280),"Settings",&ggs.white, ggs.buttonFont,arcadeModeButton,-1,-1,-1, &noAction, ggs);
    main.addButton(centeredX,scaleUI(345),"Quit To Desktop",&ggs.white, ggs.buttonFont,settingsButton,-1,-1,-1,&quitToDesktop,ggs);
    logoTexture.setup(scaleUI(454),scaleUI(92),ggs.renderer);
    logoTexture.loadFromFile("logo.png");
    main.addTitle((WINDOW_WIDTH-scaleUI(454))/2,scaleUI(100), logoTexture);

    levelSelect.setup(ggs.renderer, &buttonSound);
    const int level1Button = levelSelect.addButton(centeredX,scaleUI(225),"The Ducts",&ggs.white, ggs.buttonFont,-1,-1,-1,-1, &noAction, ggs);
    const int level2Button = levelSelect.addButton(centeredX,scaleUI(290),"Air Port",&ggs.white, ggs.buttonFont,level1Button,-1,-1,-1, &noAction, ggs);
    const int level3Button = levelSelect.addButton(centeredX,scaleUI(355),"Labratory",&ggs.white, ggs.buttonFont,level2Button,-1,-1,-1, &noAction, ggs);
    levelSelect.addButton(centeredX,scaleUI(420),"Lobby",&ggs.white, ggs.buttonFont,level3Button,-1,-1,-1, &noAction, ggs);
    levelSelect.addTitle((WINDOW_WIDTH-scaleUI(454))/2,scaleUI(100), logoTexture);
}
