#ifndef UI_H
#define UI_H
#include <SDL.h>

#include "Player.h"
#include "State.h"

void renderInGameText(bool developerMode, float lastFPS, bool waveStarted);
void updateInGameText(int playerCombo, int wave, Ability ability);

void initStartScreen();
void renderStartScreen(State& state);

void UI_init(SDL_Renderer* renderer);
void UI_close();

void initPlayerUI();
void updateTimeToShoot(double width);
void updateTimeToAbility(double width);
void renderPlayerUI(Player* player);

void mouseMove(State& state);
void mouseClick(State& state);

enum controllerMenuControl {
    connect,
    disconnect,
    up,
    down,
    select,
    back
};

void controllerEvent(State& state, controllerMenuControl control);

//Button Class
class UI_Button {

public:

    UI_Button() {}

    void setup (int _x, int _y, std::string text, SDL_Renderer* renderer);

    void render();
    int getWidth() const {return width;}

    void select() {selected = true;}
    void deselect() {selected = false;}
    bool isSelected() {return selected;}

    bool mouseEvent(int mouseX, int mouseY) const;

private:

    int x, y;

    const int width = scale(160*1.5);
    const int height = scale(32*1.5);

    Texture texture;
    Texture hoverTexture;
    Texture textTexture;

    bool selected = false;

};

#endif
