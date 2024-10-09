#ifndef UI_H
#define UI_H
#include <SDL.h>

#include "Player.h"

void renderInGameText(bool developerMode, float lastFPS, bool waveStarted);
void updateInGameText(int playerCombo, int wave);

void initStartScreen(bool controller);
void renderStartScreen();

void UI_init(SDL_Renderer* renderer);
void UI_close();

void initPlayerUI();
void updateTimeToShoot(double width);
void renderPlayerUI(Player* player);

#endif
