#ifndef INPUT_H
#define INPUT_H

#include "../TEng/Menu.h"

void mouseMove(const GlobalGameState& ggs, UI_Menu& currentMenu);

void menuSelect(UI_Menu& currentMenu);

void controllerEvent(UI_Menu& currentMenu, MENU_CONTROL control);

SDL_GameController* loadController();

void quitToDesktop(GlobalGameState& ggs, int attr1, int attr2);

void noAction(GlobalGameState& ggs, int attr1, int attr2);

#endif //INPUT_H
