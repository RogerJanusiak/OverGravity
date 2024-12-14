#ifndef INPUT_H
#define INPUT_H

#include "../includes/Input.h"
#include "../includes/Menu.h"

void mouseMove(const GlobalGameState& ggs, UI_Menu& currentMenu);

void menuSelect(UI_Menu& currentMenu);

void controllerEvent(UI_Menu& currentMenu, MENU_CONTROL control);

SDL_GameController* loadController();

#endif //INPUT_H
