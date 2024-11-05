#ifndef UI_H
#define UI_H

#include "Player.h"
#include "State.h"
#include "Menu.h"

void renderInGameText(bool developerMode, float lastFPS, bool waveStarted);
void updateInGameText(int playerCombo, int wave, int xp);

void initMenus(State &state);
void renderMenu(State & state);

void initAbilityUpgradeMenu(State& state);
void initPlayerUpgradeMenu(State& state);
void initWeaponUpgradeMenu(State& state);

void loadUpgradeMenu(State& state);
void launchUpgradeMenu();
void closeUpgradeMenu(State& state, int attr1, int attr2);

void showWeaponUpgradeMenu(State& state, int attr1, int attr2);
void showAbilityUpgradeMenu(State& state, int attr1, int attr2);
void showPlayerUpgradeMenu(State& state, int attr1, int attr2);



void UI_init(SDL_Renderer* renderer, State& state, Player* _player);
void UI_close();

void initPlayerUI();
void updateTimeToShoot(double width);
void updateTimeToAbility(double width);
void renderPlayerUI(Player* player);

void mouseMove(State& state);
void controllerEvent(State& state, MENU_CONTROL control);
void menuSelect(State& state);

#endif
