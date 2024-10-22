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
Texture abilityText;
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
    abilityText.setup(renderer);
    fpsText.setup(renderer);

    startGameText.setup(renderer);
    initPlayerUI();
}

void UI_close() {
    TTF_CloseFont(counter);
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

UI_Button arcadeModeButton;
UI_Button storyModeButton;
UI_Button settingsButton;

UI_Button level1;
UI_Button level2;

Sound buttonClick;

void initStartScreen() {
    buttonClick.init("resources/sounds/buttonClick.wav", 0,-1);
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

    for(int i = 0; i < player->getWeapon()->getClipSize(); i++) {
        if(bulletsInClip>i) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        }
        SDL_Rect tempRect;
        tempRect.x = WINDOW_WIDTH-scale(30)-scale(20*i);
        tempRect.y = WINDOW_HEIGHT-scale(25);
        tempRect.w = scale(15);
        tempRect.h = scale(15);
        SDL_RenderFillRect(renderer,&tempRect);
    }
}

void mouseClick(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(arcadeModeButton.mouseEvent(x,y) && state.mainMenu) {
        state.mainMenu = false;
        state.levelSelect = true;
        buttonClick.play();
    } else if(level1.mouseEvent(x,y) && state.levelSelect) {
        state.level = 1;
        state.levelSelect = false;
        state.started = true;
        buttonClick.play();
    } else if(level2.mouseEvent(x,y) && state.levelSelect) {
        state.level = 2;
        state.levelSelect = false;
        state.started = true;
        buttonClick.play();
    }
}

void mouseMove(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(arcadeModeButton.mouseEvent(x,y) && state.mainMenu) {
        arcadeModeButton.select();
        storyModeButton.deselect();
        settingsButton.deselect();
    } else if(storyModeButton.mouseEvent(x,y) && state.mainMenu) {
        storyModeButton.select();
        arcadeModeButton.deselect();
        settingsButton.deselect();
    } else if(settingsButton.mouseEvent(x,y) && state.mainMenu) {
        settingsButton.select();
        arcadeModeButton.deselect();
        storyModeButton.deselect();
    } else {
        if(!state.controller) {
            arcadeModeButton.deselect();
            storyModeButton.deselect();
            settingsButton.deselect();
        }
    }
    if(level1.mouseEvent(x,y) && state.levelSelect) {
        level1.select();
        level2.deselect();
    } else if(level2.mouseEvent(x,y) && state.levelSelect) {
        level2.select();
        level1.deselect();
    } else {
        if(!state.controller) {
            level1.deselect();
            level2.deselect();
        }
    }
}

bool selection1Selected = false;
bool selection2Selected = false;
UI_Button selectionNone;

void controllerEvent(State& state, controllerMenuControl control) {

    if(control == controllerMenuControl::connect && state.mainMenu) {
        arcadeModeButton.select();
    } else if(control == controllerMenuControl::connect && state.levelSelect) {
        level1.select();
    } else if(control == controllerMenuControl::disconnect) {
        arcadeModeButton.deselect();
        storyModeButton.deselect();
        settingsButton.deselect();
        level1.deselect();
        level2.deselect();
    } else if(control == controllerMenuControl::select) {
        if(arcadeModeButton.isSelected() && state.mainMenu) {
            arcadeModeButton.deselect();
            state.mainMenu = false;
            state.levelSelect = true;
            level1.select();
            buttonClick.play();
        } else if(level1.isSelected() && state.levelSelect) {
            level1.deselect();
            state.levelSelect = false;
            state.started = true;
            state.level = 1;
            buttonClick.play();
        } else if(level2.isSelected() && state.levelSelect) {
            level2.deselect();
            state.levelSelect = false;
            state.started = true;
            state.level = 2;
            buttonClick.play();
        }
    } else if(control == controllerMenuControl::back) {
        if(state.levelSelect) {
            level1.deselect();
            level2.deselect();
            arcadeModeButton.select();
            state.levelSelect = false;
            state.mainMenu = true;
            buttonClick.play();
        }
    } else {
        if(state.mainMenu) {
            if(control == controllerMenuControl::down) {
                if(arcadeModeButton.isSelected()) {
                    arcadeModeButton.deselect();
                    storyModeButton.select();
                } else if(storyModeButton.isSelected()) {
                    storyModeButton.deselect();
                    settingsButton.select();
                }
            } else if(control == controllerMenuControl::up) {
                if(storyModeButton.isSelected()) {
                    storyModeButton.deselect();
                    arcadeModeButton.select();
                } else if(settingsButton.isSelected()) {
                    settingsButton.deselect();
                    storyModeButton.select();
                }
            }
        } else if(state.levelSelect) {
            if(control == controllerMenuControl::down) {
                if(level1.isSelected()) {
                    level1.deselect();
                    level2.select();
                }
            } else if(control == controllerMenuControl::up) {
                if(level2.isSelected()) {
                    level2.deselect();
                    level1.select();
                }
            }
        } else if(state.upgradeScreen) {
            if(control == controllerMenuControl::left && selection2Selected) {
                selection2Selected = false;
                selection1Selected = true;
            } else if(control == controllerMenuControl::right && selection1Selected) {
                selection2Selected = true;
                selection1Selected = false;
            } else if(control == controllerMenuControl::down) {
                selection2Selected = false;
                selection1Selected = false;
                selectionNone.select();
            } else if(control == controllerMenuControl::up && selectionNone.isSelected()) {
                selection2Selected = false;
                selection1Selected = true;
                selectionNone.deselect();
            }
        }
    }

}

void UI_Button::setup(const int _x, const int _y, std::string text, SDL_Renderer* renderer) {
    x = _x;
    y = _y;
    texture.setup(width,height,renderer);
    texture.loadFromFile("button.png");

    hoverTexture.setup(width,height,renderer);
    hoverTexture.loadFromFile("button1.png");

    textTexture.setup(0,0,renderer);
    textTexture.loadFromRenderedText(text, white, counter);
}

bool UI_Button::mouseEvent(const int mouseX, const int mouseY) const {
    if(mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height) {
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
    textTexture.render(x+(width-textTexture.getWidth())/2,y+(height-textTexture.getHeight())/2);
}

Texture selectionTexture;
Texture currentSetupText;
Texture currentAbilityText;

Texture selectionBackground;
Texture selectionBackgroundSelected;
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

void initSelectionUI() {

    selectionTexture.setup(renderer);
    selectionTexture.loadFromRenderedText("Select new weapon or upgrade: ", white, title);

    currentSetupText.setup(renderer);
    currentAbilityText.setup(renderer);

    selectionBackground.setup(selectWidth, selectWidth,renderer);
    selectionBackground.loadFromFile("upgrade-background.png");

    selectionBackgroundSelected.setup(selectWidth, selectWidth,renderer);
    selectionBackgroundSelected.loadFromFile("upgrade-background-selected.png");

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

    if(selection1Selected) {
        selectionBackgroundSelected.render(select1X,selectY);
    } else {
        selectionBackground.render(select1X,selectY);
    }

    if(selection2Selected) {
        selectionBackgroundSelected.render(select2X,selectY);
    } else {
        selectionBackground.render(select2X,selectY);
    }

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
//TODO: Make the button class more robust to be able to add in these buttons
void updateChoices(State& state, Weapon *_weapon1, Weapon *_weapon2, Ability _ability1, Ability _ability2) {
    weapon1 = _weapon1;
    weapon2 = _weapon2;
    ability1 = _ability1;
    ability2 = _ability2;
    if(state.controller) {
        selection1Selected = true;
        selection2Selected = false;
    }
}

int selectionMouseClick() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(x >= select1X && x <= select1X+selectWidth && y >= selectY && y <= selectY+selectWidth) {
        return 1;
    }
    if(x >= select2X && x <= select2X+selectWidth && y >= selectY && y <= selectY+selectWidth) {
        return 2;
    }
    if(selectionNone.mouseEvent(x,y)) {
        selectionNone.deselect();
        return 0;
    }
    return -1;
}

void selectionMouseMove(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );
    if(x >= select1X && x <= select1X+selectWidth && y >= selectY && y <= selectY+selectWidth) {
        selection1Selected = true;
        selection2Selected = false;
        selectionNone.deselect();
    } else if(x >= select2X && x <= select2X+selectWidth && y >= selectY && y <= selectY+selectWidth) {
        selection2Selected = true;
        selection1Selected = false;
        selectionNone.deselect();
    } else if(selectionNone.mouseEvent(x,y)) {
        selection2Selected = false;
        selection1Selected = false;
        selectionNone.select();
    } else {
        if(!state.controller) {
            selection1Selected = false;
            selection2Selected = false;
            selectionNone.deselect();
        }
    }
}

int selectionControllerClick() {
    if(selection1Selected) {
        return 1;
    }
    if(selection2Selected) {
        return 2;
    }
    if(selectionNone.isSelected()) {
        selectionNone.deselect();
        return 0;
    }
    return -1;
}