#include "../../includes/Utils/Input.h"
#include "../../includes/TEng/Menu.h"

void mouseMove(const GlobalGameState& ggs, UI_Menu& currentMenu) {
    int x, y;
    SDL_GetMouseState( &x, &y );

    if(ggs.controller == nullptr) {
        if(currentMenu.currentButton != nullptr) {
            currentMenu.currentButton->deselect();
            currentMenu.currentButton = nullptr;
        }
    }

    std::vector<UI_Button>* menuButtons = currentMenu.getButtons();
    for(auto & menuButton : *menuButtons) {
        if(menuButton.mouseEvent(x,y) && !menuButton.isDisabled()) {
            if(currentMenu.currentButton != nullptr) {
                currentMenu.currentButton->deselect();
            }
            currentMenu.currentButton = &menuButton;
            currentMenu.currentButton->select();
        }
    }
}

void menuSelect(UI_Menu& currentMenu) {
    std::vector<UI_Button>* menuButtons = currentMenu.getButtons();
    for(auto & menuButton : *menuButtons) {
        if(menuButton.isSelected()) {
            menuButton.click();
        }
    }
}

void controllerEvent(UI_Menu& currentMenu, MENU_CONTROL control) {
    switch(control) {
        case MENU_CONTROL::connect:
            currentMenu.loadMenu();
            break;
        case MENU_CONTROL::disconnect: {
            if(currentMenu.currentButton != nullptr) {
                currentMenu.currentButton->deselect();
                currentMenu.currentButton = nullptr;
            }
        } break;
        case MENU_CONTROL::select:
            menuSelect(currentMenu);
            break;
        default: {
            if(currentMenu.currentButton != nullptr) {
                UI_Button* tempButton = currentMenu.currentButton->move(control);
                if(tempButton != nullptr) {
                    currentMenu.currentButton->deselect();
                    currentMenu.currentButton = tempButton;
                    currentMenu.currentButton->select();
                }
            } else {
                SDL_Log("No button selected!");
            }
        } break;
    }
}

SDL_GameController* loadController() {
    SDL_GameController *controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
    if(controller != nullptr) {
        SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
    }
    return controller;
}

void quitToDesktop(GlobalGameState& ggs, int attr1, int attr2) {
    ggs.quit = true;
}

void noAction(GlobalGameState& ggs, int attr1, int attr2) {}