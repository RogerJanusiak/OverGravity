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

void initSelectionUI();
void updateChoices(State& state, Weapon* _weapon1, Weapon* _weapon2, Ability _ability1, Ability _ability2);
void renderSelectionUI(Weapon* currentWeapon, Ability currentAbility);

void initPlayerUI();
void updateTimeToShoot(double width);
void updateTimeToAbility(double width);
void renderPlayerUI(Player* player);
int selectionMouseClick();
void selectionMouseMove(State& state);
int selectionControllerClick();

void initPauseScreen();
void renderPauseScreen();

void mouseMove(State& state);
void mouseClick(State& state);

void selectAction(State& state);

enum controllerMenuControl {
    connect,
    disconnect,
    up,
    down,
    select,
    back,
    left,
    right,
};

void controllerEvent(State& state, controllerMenuControl control);

//Button Class
class UI_Button {

public:

    UI_Button() {}

    void setup (int _x, int _y, std::string text, SDL_Renderer* renderer, int _type = 0);

    void render();
    int getWidth() const {return width;}

    [[nodiscard]] UI_Button* move(controllerMenuControl action) const;
    void linkButtons(UI_Button* above, UI_Button* below,UI_Button* left, UI_Button* right) { buttonAbove = above; buttonBelow = below; buttonLeft = left; buttonRight = right; }

    void select() {selected = true;}
    void deselect() {selected = false;}
    bool isSelected() const {return selected;}

    bool mouseEvent(int mouseX, int mouseY) const;

private:

    int x, y;

    int type;

    const int width = scale(160*1.5);
    const int height = scale(32*1.5);

    const int sWidth = scale(200);
    const int sHeight = scale(200);

    UI_Button* buttonAbove = nullptr;
    UI_Button* buttonBelow = nullptr;
    UI_Button* buttonRight = nullptr;
    UI_Button* buttonLeft = nullptr;

    Texture texture;
    Texture hoverTexture;
    Texture textTexture;

    bool selected = false;

};

#endif
