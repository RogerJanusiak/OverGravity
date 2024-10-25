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
TTF_Font *small;

Texture waveNumberText;
Texture waveNumberTitle;
Texture comboNumberText;
Texture abilityText;
Texture fpsText;

UI_Button* currentButton = nullptr;

Texture logoTexture;

Texture startGameText;

SDL_Renderer* renderer;

Texture selectionTexture;
Texture currentSetupText;
Texture currentAbilityText;

Texture laserPistolSelectTexture;
Texture knifeSelectTexture;
Texture bounceSelectTexture;
Texture c4SelectTexture;
Texture teleportSelectTexture;

Weapon* weapon1;
Weapon* weapon2;
Ability ability1;
Ability ability2;

int selectWidth = scale(200);
int selectSpacing = scale(50);
int selectY = (WINDOW_HEIGHT-selectWidth)/2;
int select1X = WINDOW_WIDTH/2 - selectWidth - selectSpacing/2;
int select2X = WINDOW_WIDTH/2 + selectSpacing/2;

UI_Button arcadeModeButton;
UI_Button storyModeButton;
UI_Button settingsButton;

UI_Button level1;
UI_Button level2;

UI_Button selection1;
UI_Button selection2;

Sound buttonClick;

SDL_Rect timeToShootBack;
SDL_Rect timeToShoot;

SDL_Rect timeToAbilityBack;
SDL_Rect timeToAbility;

Texture ammoLeftText;

UI_Button resumeGameButton;
UI_Button quitToMenuGameButton;
UI_Button quitToDesktopGameButton;

UI_Button quitToDesktopMainButton;

UI_Button selectionNone;
Texture gamePausedText;

void UI_init(SDL_Renderer* _renderer) {
    counter = TTF_OpenFont("resources/sans.ttf",scale(18));
    small = TTF_OpenFont("resources/sans.ttf",scale(12));
    title = TTF_OpenFont("resources/sans.ttf",scale(34));

    renderer = _renderer;

    waveNumberText.setup(renderer);
    waveNumberTitle.setup(renderer);
    comboNumberText.setup(renderer);
    abilityText.setup(renderer);
    fpsText.setup(renderer);

    startGameText.setup(renderer);
    initPlayerUI();

    initStartScreen();
    initSelectionUI();
    initPauseScreen();

}

void UI_close() {
    TTF_CloseFont(counter);
    TTF_CloseFont(small);
    TTF_CloseFont(title);
}

void updateInGameText(int playerCombo, int wave, Ability ability) {
    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, counter);
    waveNumberText.loadFromRenderedText("Wave: " + std::to_string(wave), white, counter);
    waveNumberTitle.loadFromRenderedText("Wave " + std::to_string(wave) + " Start!", white, title);
    switch(ability) {
        case bounce:
            abilityText.loadFromRenderedText("Ability: Bounce", white, counter);
            break;
        case respawn:
            abilityText.loadFromRenderedText("Ability: Respawn", white, counter);
            break;
        case c4:
            abilityText.loadFromRenderedText("Ability: C4", white, counter);
            break;
        default:
            abilityText.loadFromRenderedText("Ability: None", white, counter);
    }

}

void renderInGameText(bool developerMode, float lastFPS,bool waveStarted) {
    waveNumberText.render(scale(10),scale(5));
    comboNumberText.render(scale(10),scale(30));
    abilityText.render(scale(10),scale(55));
    if(developerMode) {
        fpsText.loadFromRenderedText("FPS: " + std::to_string(lastFPS), white, counter);
        fpsText.render(scale(10),scale(80));
    }
    if(!waveStarted) {
        waveNumberTitle.render((WINDOW_WIDTH-waveNumberTitle.getWidth())/2,scale(200));
    }
}

void initStartScreen() {
    buttonClick.init("resources/sounds/buttonClick.wav", 0,-1);
    logoTexture.setup(scale(454),scale(92),renderer);
    logoTexture.loadFromFile("logo.png");
    arcadeModeButton.setup((WINDOW_WIDTH-arcadeModeButton.getWidth())/2,scale(215),"Arcade Mode", renderer);
    storyModeButton.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(280),"Story Mode", renderer);
    settingsButton.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(345),"Settings", renderer);
    quitToDesktopMainButton.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(410),"Quit To Desktop", renderer);
    level1.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(215),"Level 1", renderer);
    level2.setup((WINDOW_WIDTH-storyModeButton.getWidth())/2,scale(280),"Level 2", renderer);
    arcadeModeButton.linkButtons(nullptr,&storyModeButton,nullptr,nullptr);
    storyModeButton.linkButtons(&arcadeModeButton,&settingsButton,nullptr,nullptr);
    settingsButton.linkButtons(&storyModeButton,&quitToDesktopMainButton,nullptr,nullptr);
    quitToDesktopMainButton.linkButtons(&settingsButton,nullptr,nullptr,nullptr);
    level1.linkButtons(nullptr,&level2,nullptr,nullptr);
    level2.linkButtons(&level1,nullptr,nullptr,nullptr);
}

void renderStartScreen(State& state) {
    logoTexture.render((WINDOW_WIDTH-logoTexture.getWidth())/2,scale(100));
    if(state.mainMenu) {
        arcadeModeButton.render();
        storyModeButton.render();
        settingsButton.render();
        quitToDesktopMainButton.render();
    } else if(state.levelSelect) {
        level1.render();
        level2.render();
    }

}

void initPlayerUI() {
    timeToShootBack.x = WINDOW_WIDTH-scale(90);
    timeToShootBack.y = WINDOW_HEIGHT-scale(50);
    timeToShootBack.w = scale(75);
    timeToShootBack.h = scale(15);

    timeToShoot.x = WINDOW_WIDTH-scale(90);
    timeToShoot.y = WINDOW_HEIGHT-scale(50);
    timeToShoot.w = scale(75);
    timeToShoot.h = scale(15);

    timeToAbilityBack.x = WINDOW_WIDTH-scale(90);
    timeToAbilityBack.y = WINDOW_HEIGHT-scale(75);
    timeToAbilityBack.w = scale(75);
    timeToAbilityBack.h = scale(15);

    timeToAbility.x = WINDOW_WIDTH-scale(90);
    timeToAbility.y = WINDOW_HEIGHT-scale(75);
    timeToAbility.w = scale(75);
    timeToAbility.h = scale(15);

    ammoLeftText.setup(renderer);

}

void updateTimeToShoot(const double width) {
    timeToShoot.w = width;
}

void updateTimeToAbility(const double width) {
    timeToAbility.w = width;
}

void renderPlayerUI(Player* player) {
    int bulletsInClip = player->getWeapon()->getBulletsInClip();
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer,&timeToShootBack);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&timeToShoot);

    if(player->getAbility() == respawn) {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderFillRect(renderer,&timeToAbilityBack);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer,&timeToAbility);
    }

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
        case 3: {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&player->playerHealth1);
            SDL_RenderFillRect(renderer,&player->playerHealth2);
            SDL_RenderFillRect(renderer,&player->playerHealth3);
        } break;
        case 2: {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth1);
                SDL_RenderFillRect(renderer,&player->playerHealth2);
                SDL_SetRenderDrawColor(renderer, 170, 104, 95, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth3);
        } break;
        case 1: {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth1);
                SDL_SetRenderDrawColor(renderer, 170, 104, 95, 255);
                SDL_RenderFillRect(renderer,&player->playerHealth2);
                SDL_RenderFillRect(renderer,&player->playerHealth3);
        } break;
        default:
            break;
    }

    for(int i = 0; i < player->getWeapon()->getClipSize()+1; i++) {
        if(i == 0) {
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        } else {
            if(bulletsInClip>i-1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }
        }
        SDL_Rect tempRect;
        tempRect.x = WINDOW_WIDTH-scale(30)-scale(20*i);
        tempRect.y = WINDOW_HEIGHT-scale(25);
        tempRect.w = scale(15);
        tempRect.h = scale(15);
        SDL_RenderFillRect(renderer,&tempRect);
    }

    if(player->getWeapon()->getClipSize() != 0) {

        if(player->getWeapon()->getType() == revolver) {
            ammoLeftText.loadFromRenderedText("--",white,small);
            ammoLeftText.render(WINDOW_WIDTH-scale(26),WINDOW_HEIGHT-scale(29));
        } else {
            int ammo = player->getWeapon()->getBulletsLeft();
            ammo += player->getWeapon()->getClipSize()-bulletsInClip;
            ammo -= player->getWeapon()->getClipSize();
            ammoLeftText.loadFromRenderedText(std::to_string(ammo),white,small);
            if(ammo < 10) {
                ammoLeftText.render(WINDOW_WIDTH-scale(25),WINDOW_HEIGHT-scale(27));
            } else {
                ammoLeftText.render(WINDOW_WIDTH-scale(29),WINDOW_HEIGHT-scale(27));
            }
        }

    }

    if(player->getAbility() == c4) {
        for(int i = 0; i < 3; i++) {
            if(player->getC4Left()>i) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }
            SDL_Rect tempRect;
            tempRect.x = WINDOW_WIDTH-scale(30)-scale(20*i);
            tempRect.y = WINDOW_HEIGHT-scale(75);
            tempRect.w = scale(15);
            tempRect.h = scale(15);
            SDL_RenderFillRect(renderer,&tempRect);
        }
    }

}

void mouseClick(State& state) {
    selectAction(state);
}

void mouseMove(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );
    UI_Button* tempButton = nullptr;
    if(arcadeModeButton.mouseEvent(x,y) && state.mainMenu) {
        tempButton = &arcadeModeButton;
    } else if(storyModeButton.mouseEvent(x,y) && state.mainMenu) {
        tempButton = &storyModeButton;
    } else if(settingsButton.mouseEvent(x,y) && state.mainMenu) {
        tempButton = &settingsButton;
    } else if(quitToDesktopMainButton.mouseEvent(x,y) && state.mainMenu) {
        tempButton = &quitToDesktopMainButton;
    }
    if(level1.mouseEvent(x,y) && state.levelSelect) {
        tempButton = &level1;
    } else if(level2.mouseEvent(x,y) && state.levelSelect) {
        tempButton = &level2;
    }
    if(resumeGameButton.mouseEvent(x,y) && state.paused) {
        tempButton = &resumeGameButton;
    } else if(quitToMenuGameButton.mouseEvent(x,y) && state.paused) {
        tempButton = &quitToMenuGameButton;
    } else if(quitToDesktopGameButton.mouseEvent(x,y) && state.paused) {
        tempButton = &quitToDesktopGameButton;
    }
    if(selection1.mouseEvent(x,y) && state.upgradeScreen) {
        tempButton = &selection1;
    } else if(selection2.mouseEvent(x,y) && state.upgradeScreen) {
        tempButton = &selection2;
    } else if(selectionNone.mouseEvent(x,y) && state.upgradeScreen) {
        tempButton = &selectionNone;
    }
    if(currentButton != nullptr && !state.controller) {
        currentButton->deselect();
    }
    if(tempButton != nullptr) {
        if(currentButton != nullptr) {
            currentButton->deselect();
        }
        currentButton = tempButton;
        currentButton->select();
    }
}

void initSelectionUI() {

    selectionTexture.setup(renderer);
    selectionTexture.loadFromRenderedText("Select new weapon or upgrade: ", white, title);

    currentSetupText.setup(renderer);
    currentAbilityText.setup(renderer);

    selection1.setup(select1X,selectY," ", renderer,1);
    selection2.setup(select2X,selectY," ", renderer,1);

    selection1.linkButtons(nullptr,&selectionNone,nullptr,&selection2);
    selection2.linkButtons(nullptr,&selectionNone,&selection1,nullptr);
    selectionNone.linkButtons(&selection1,nullptr,nullptr,nullptr);

    laserPistolSelectTexture.setup(selectWidth, selectWidth,renderer);
    laserPistolSelectTexture.loadFromFile("upgrade-laserPistol.png");

    knifeSelectTexture.setup(selectWidth, selectWidth,renderer);
    knifeSelectTexture.loadFromFile("upgrade-knife.png");

    bounceSelectTexture.setup(selectWidth, selectWidth,renderer);
    bounceSelectTexture.loadFromFile("upgrade-bounce.png");

    teleportSelectTexture.setup(selectWidth, selectWidth,renderer);
    teleportSelectTexture.loadFromFile("upgrade-teleport.png");

    c4SelectTexture.setup(selectWidth, selectWidth,renderer);
    c4SelectTexture.loadFromFile("upgrade-c4.png");

    selectionNone.setup(WINDOW_WIDTH/2-selectionNone.getWidth()/2, selectY+scale(20)+selectWidth,"Keep Current Setup",renderer);

}

void renderSelectionUI(Weapon* currentWeapon, Ability currentAbility) {

    selectionTexture.render(select1X/2,selectY/2);
    selectionNone.render();

    selection1.render();
    selection2.render();

    if(weapon1 != nullptr) {
        if(weapon1->getType() == knife) {
            knifeSelectTexture.render(select1X,selectY);
        } else if(weapon1->getType() == laserPistol) {
            laserPistolSelectTexture.render(select1X,selectY);
        }
    } else {
        if(ability1 == c4) {
            c4SelectTexture.render(select1X,selectY);
        } else if(ability1 == respawn) {
            teleportSelectTexture.render(select1X,selectY);
        } else if(ability1 == bounce) {
            bounceSelectTexture.render(select1X,selectY);
        }
    }
    if(weapon2 != nullptr) {
        if(weapon2->getType() == knife) {
            knifeSelectTexture.render(select2X,selectY);
        } else if(weapon2->getType() == laserPistol) {
            laserPistolSelectTexture.render(select2X,selectY);
        }
    } else {
        if(ability2 == c4) {
            c4SelectTexture.render(select2X,selectY);
        } else if(ability2 == respawn) {
            teleportSelectTexture.render(select2X,selectY);
        } else if(ability2 == bounce) {
            bounceSelectTexture.render(select2X,selectY);
        }
    }

    if(currentWeapon != nullptr) {
        if(currentWeapon->getType() == knife) {
            currentSetupText.loadFromRenderedText("Current Weapon: Knife", white, counter);
        } else if(currentWeapon->getType() == laserPistol) {
            currentSetupText.loadFromRenderedText("Current Weapon: Laser Pistol", white, counter);
        }
    } else {
        currentSetupText.loadFromRenderedText("Current Weapon: None", white, counter);
    }

    if(currentAbility == none) {
        currentAbilityText.loadFromRenderedText("Current Ability: None", white, counter);
    } else {
        if(currentAbility == c4) {
            currentAbilityText.loadFromRenderedText("Current Ability: C4", white, counter);
        } else if(currentAbility == respawn) {
            currentAbilityText.loadFromRenderedText("Current Ability: Teleport", white, counter);
        } else if(currentAbility == bounce) {
            currentAbilityText.loadFromRenderedText("Current Ability: Bounce", white, counter);
        }
    }
    currentSetupText.render(scale(15),WINDOW_HEIGHT-scale(40));
    currentAbilityText.render(scale(15),WINDOW_HEIGHT-scale(65));
}

void updateChoices(State& state, Weapon *_weapon1, Weapon *_weapon2, Ability _ability1, Ability _ability2) {
    weapon1 = _weapon1;
    weapon2 = _weapon2;
    ability1 = _ability1;
    ability2 = _ability2;
    if(state.controller) {
        currentButton = &selection1;
        selection1.select();
        selection2.deselect();
    }
}

int selectionMouseClick() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(selection1.isSelected()) {
        return 1;
    }
    if(selection2.isSelected()) {
        return 2;
    }
    if(selectionNone.mouseEvent(x,y)) {
        selectionNone.deselect();
        return 0;
    }
    return -1;
}

int selectionControllerClick() {
    if(selection1.isSelected()) {
        return 1;
    }
    if(selection2.isSelected()) {
        return 2;
    }
    if(selectionNone.isSelected()) {
        selectionNone.deselect();
        return 0;
    }
    return -1;
}

void controllerEvent(State& state, controllerMenuControl control) {

        if(control == connect) {
            if(state.mainMenu) {
                arcadeModeButton.select();
                currentButton = &arcadeModeButton;
            } else if(state.levelSelect) {
                level1.select();
                currentButton = &level1;
            } else if(state.paused) {
                resumeGameButton.select();
                currentButton = &resumeGameButton;
            }
        } else if(control == disconnect) {
                currentButton->deselect();
                currentButton = nullptr;
        } else if(control == select) {
            selectAction(state);
        } else {
            if(currentButton != nullptr) {
                UI_Button* tempButton = currentButton->move(control);
                if(tempButton != nullptr) {
                    currentButton->deselect();
                    currentButton = tempButton;
                    currentButton->select();
                }
            } else {
                SDL_Log("No button selection!");
            }
        }

}

void selectAction(State& state) {
    if(state.mainMenu) {
        if(arcadeModeButton.isSelected()) {
            state.mainMenu = false;
            state.levelSelect = true;
            currentButton = &level1;
        } else if(quitToDesktopMainButton.isSelected()) {
            state.quit = true;
        }
        if(currentButton != nullptr) {
            currentButton->select();
        }
    } else if(state.levelSelect) {
        state.levelSelect = false;
        state.started = true;
        if(level1.isSelected()) {
            state.level = 1;
        } else if(level2.isSelected()) {
            state.level = 2;
        }
        if(currentButton != nullptr) {
            currentButton->deselect();
            currentButton = nullptr;
        }
    } else if(state.paused) {
        state.paused = false;
        if(resumeGameButton.isSelected()) {
            currentButton->deselect();
            currentButton = nullptr;
        } else if(quitToMenuGameButton.isSelected()) {
            state.started = false;
            state.mainMenu = true;
            currentButton->deselect();
            currentButton = &arcadeModeButton;
            currentButton->select();
        } else if(quitToDesktopGameButton.isSelected()) {
            state.quit = true;
        }
    }
    buttonClick.play();
}

void initPauseScreen() {
    gamePausedText.setup(renderer);
    gamePausedText.loadFromRenderedText("Game Paused", white, title);

    resumeGameButton.setup((WINDOW_WIDTH-arcadeModeButton.getWidth())/2,scale(215),"Resume Game",renderer);
    quitToMenuGameButton.setup((WINDOW_WIDTH-arcadeModeButton.getWidth())/2,scale(280),"Quit to Main Menu",renderer);
    quitToDesktopGameButton.setup((WINDOW_WIDTH-arcadeModeButton.getWidth())/2,scale(345),"Quit to Desktop",renderer);
    resumeGameButton.linkButtons(nullptr,&quitToMenuGameButton,nullptr,nullptr);
    quitToMenuGameButton.linkButtons(&resumeGameButton,&quitToDesktopGameButton,nullptr,nullptr);
    quitToDesktopGameButton.linkButtons(&quitToMenuGameButton,nullptr,nullptr,nullptr);
}

void renderPauseScreen() {
    gamePausedText.render((WINDOW_WIDTH-gamePausedText.getWidth())/2,scale(100));
    resumeGameButton.render();
    quitToMenuGameButton.render();
    quitToDesktopGameButton.render();
}

// UI_Button Class Member Functions

void UI_Button::setup(const int _x, const int _y, std::string text, SDL_Renderer* renderer, int _type) {
    x = _x;
    y = _y;
    type = _type;
    if(type == 0) {
        texture.setup(width,height,renderer);
        texture.loadFromFile("button.png");

        hoverTexture.setup(width,height,renderer);
        hoverTexture.loadFromFile("button1.png");
    } else if(type == 1) {
        texture.setup(sWidth,sHeight,renderer);
        texture.loadFromFile("upgrade-background.png");

        hoverTexture.setup(sWidth,sHeight,renderer);
        hoverTexture.loadFromFile("upgrade-background-selected.png");
    }

    textTexture.setup(0,0,renderer);
    textTexture.loadFromRenderedText(text, white, counter);
}

bool UI_Button::mouseEvent(const int mouseX, const int mouseY) const {
    int w;
    int h;
    if(type == 0) {
        w  = width;
        h = height;
    } else if(type == 1) {
        w = sWidth;
        h = sHeight;
    }
    if(mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h) {
        return true;
    }
    return false;
}

void UI_Button::render() {
    if(selected) {
        hoverTexture.render(x,y);
    } else {
        texture.render(x,y);
    }
    if(type == 0) {
        textTexture.render(x+(width-textTexture.getWidth())/2,y+(height-textTexture.getHeight())/2);
    }
}

UI_Button* UI_Button::move(const controllerMenuControl action) const {
    switch(action) {
    case up:
        return buttonAbove;
    case down:
        return buttonBelow;
    case left:
        return buttonLeft;
    case right:
        return buttonRight;
    default:
        return nullptr;
    }
}

