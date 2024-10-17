#include "../includes/UI.h"
#include "../includes/Player.h"
#include "../includes/GlobalConstants.h"

#include <SDL_ttf.h>
#include <string>

#include "../includes/Texture.h"

SDL_Color white = { 255, 255, 255 };
SDL_Color gray = { 105, 105, 105 };
TTF_Font *counter;
TTF_Font *title;

Texture waveNumberText;
Texture waveNumberTitle;
Texture comboNumberText;
Texture fpsText;

Texture logoTexture;

Texture startGameText;

SDL_Renderer* renderer;

void UI_init(SDL_Renderer* _renderer) {
    counter = TTF_OpenFont("resources/sans.ttf",scale(18));
    title = TTF_OpenFont("resources/sans.ttf",scale(34));

    renderer = _renderer;

    waveNumberText.setup(renderer);
    waveNumberTitle.setup(renderer);
    comboNumberText.setup(renderer);
    fpsText.setup(renderer);

    startGameText.setup(renderer);
    initPlayerUI();
}

void UI_close() {
    TTF_CloseFont(counter);
    TTF_CloseFont(title);
}

void updateInGameText(int playerCombo, int wave) {
    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, counter);
    waveNumberText.loadFromRenderedText("Wave: " + std::to_string(wave), white, counter);
    waveNumberTitle.loadFromRenderedText("Wave " + std::to_string(wave) + " Start!", white, title);
}

void renderInGameText(bool developerMode, float lastFPS,bool waveStarted) {
    waveNumberText.render(scale(10),scale(5));
    comboNumberText.render(scale(10),scale(30));
    if(developerMode) {
        fpsText.loadFromRenderedText("FPS: " + std::to_string(lastFPS), white, counter);
        fpsText.render(scale(10),scale(55));
    }
    if(!waveStarted) {
        waveNumberTitle.render((WINDOW_WIDTH-waveNumberTitle.getWidth())/2,scale(200));
    }
}

UI_Button arcadeModeButton;
UI_Button storyModeButton;
UI_Button settingsButton;

UI_Button level1;
UI_Button level2;

void initStartScreen(bool controller) {
    logoTexture.setup(scale(454),scale(92),renderer);
    logoTexture.loadFromFile("logo.png");
    arcadeModeButton.setup((WINDOW_WIDTH-arcadeModeButton.getWidth())/2,scale(215),"Arcade Mode", renderer);
    storyModeButton.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(280),"Story Mode", renderer);
    settingsButton.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(345),"Settings", renderer);
    level1.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(215),"Level 1", renderer);
    level2.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(280),"Level 2", renderer);
}

void renderStartScreen(State& state) {
    logoTexture.render((WINDOW_WIDTH-logoTexture.getWidth())/2,scale(100));
    if(state.mainMenu) {
        arcadeModeButton.render();
        storyModeButton.render();
        settingsButton.render();
    } else if(state.levelSelect) {
        level1.render();
        level2.render();
    }

}


SDL_Rect timeToShootBack;
SDL_Rect timeToShoot;

SDL_Rect timeToAbilityBack;
SDL_Rect timeToAbility;

void initPlayerUI() {
    timeToShootBack.x = WINDOW_WIDTH-scale(90);
    timeToShootBack.y = WINDOW_HEIGHT-scale(25);
    timeToShootBack.w = scale(75);
    timeToShootBack.h = scale(15);

    timeToShoot.x = WINDOW_WIDTH-scale(90);
    timeToShoot.y = WINDOW_HEIGHT-scale(25);
    timeToShoot.w = scale(75);
    timeToShoot.h = scale(15);

    timeToAbilityBack.x = WINDOW_WIDTH-scale(90);
    timeToAbilityBack.y = WINDOW_HEIGHT-scale(45);
    timeToAbilityBack.w = scale(75);
    timeToAbilityBack.h = scale(15);

    timeToAbility.x = WINDOW_WIDTH-scale(90);
    timeToAbility.y = WINDOW_HEIGHT-scale(45);
    timeToAbility.w = scale(75);
    timeToAbility.h = scale(15);
}

void updateTimeToShoot(const double width) {
    timeToShoot.w = width;
}

void updateTimeToAbility(const double width) {
    timeToAbility.w = width;
}

void renderPlayerUI(Player* player) {
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer,&timeToShootBack);
    SDL_RenderFillRect(renderer,&timeToAbilityBack);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&timeToShoot);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer,&timeToAbility);

    switch(player->getShield()) {
        case 2: {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer,&player->playerShield1);
                SDL_RenderFillRect(renderer,&player->playerShield2);
        } break;
        case 1: {
                SDL_SetRenderDrawColor(renderer, 183, 201, 226, 255);
                SDL_RenderFillRect(renderer,&player->playerShield2);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer,&player->playerShield1);
        } break;
        default: {
                SDL_SetRenderDrawColor(renderer, 183, 201, 226, 255);
                SDL_RenderFillRect(renderer,&player->playerShield1);
                SDL_RenderFillRect(renderer,&player->playerShield2);
        } break;
    }

    switch(player->getHP()) {
        case 2: {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth1);
                SDL_RenderFillRect(renderer,&player->playerHealth2);
        } break;
        case 1: {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth1);
                SDL_SetRenderDrawColor(renderer, 170, 104, 95, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth2);
        } break;
        default:
            break;
    }

}

void mouseMove(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );

}

void mouseClick(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(arcadeModeButton.mouseEvent(x,y) && state.mainMenu) {
        state.mainMenu = false;
        state.levelSelect = true;
    } else if(level1.mouseEvent(x,y) && state.levelSelect) {
        state.level = 1;
        state.levelSelect = false;
        state.started = true;
    } else if(level2.mouseEvent(x,y) && state.levelSelect) {
        state.level = 2;
        state.levelSelect = false;
        state.started = true;
    }
}


void UI_Button::setup(const int _x, const int _y, std::string text, SDL_Renderer* renderer) {
    x = _x;
    y = _y;
    texture.setup(width,height,renderer);
    texture.loadFromFile("button.png");

    textTexture.setup(0,0,renderer);
    textTexture.loadFromRenderedText(text, white, counter);
}

bool UI_Button::mouseEvent(const int mouseX, const int mouseY) const {
    if(mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height) {
        return true;
    }
    return false;
}


