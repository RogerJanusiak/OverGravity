#ifndef UI_H
#define UI_H
#include <SDL.h>

#include "Player.h"
#include "State.h"

void renderInGameText(bool developerMode, float lastFPS, bool waveStarted);
void updateInGameText(int playerCombo, int wave);

void initStartScreen(bool controller);
void renderStartScreen(State& state);

void UI_init(SDL_Renderer* renderer);
void UI_close();

void initPlayerUI();
void updateTimeToShoot(double width);
void renderPlayerUI(Player* player);

void mouseMove(State& state);
void mouseClick(State& state);


//Button Class
class UI_Button {

public:

    UI_Button() {}

    void setup (int _x, int _y, std::string text, SDL_Renderer* renderer);

    void render() const {texture.render(x,y); textTexture.render(x+(width-textTexture.getWidth())/2,y+(height-textTexture.getHeight())/2); }
    int getWidth() const {return width;}

    bool mouseEvent(int mouseX, int mouseY) const;

private:

    int x, y;

    const int width = scale(160*1.5);
    const int height = scale(32*1.5);

    Texture texture;
    Texture textTexture;

    bool selected = false;

};

#endif
