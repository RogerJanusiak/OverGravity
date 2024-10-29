#ifndef UI_H
#define UI_H

#include "Player.h"
#include "State.h"
#include "Menu.h"

void renderInGameText(bool developerMode, float lastFPS, bool waveStarted);
void updateInGameText(int playerCombo, int wave, Ability ability);

void initMenus(State &state);
void renderMenu(State & state);
void initWeaponUpgradeMenu(State& state);

void loadUpgradeMenu(State& state);

void UI_init(SDL_Renderer* renderer, State& state);
void UI_close();

void initPlayerUI();
void updateTimeToShoot(double width);
void updateTimeToAbility(double width);
void renderPlayerUI(Player* player);

void mouseMove(State& state);
void controllerEvent(State& state, MENU_CONTROL control);
void menuSelect(State& state);

#endif
