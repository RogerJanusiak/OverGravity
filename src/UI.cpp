#include "../includes/UI.h"
#include "../includes/GlobalConstants.h"
#include "../includes/State.h"

#include <SDL_ttf.h>
#include <string>

#include "../includes/Texture.h"

SDL_Color white = { 255, 255, 255 };
SDL_Color gray = { 105, 105, 105 };
TTF_Font *counter;
TTF_Font *title;
TTF_Font *small;
TTF_Font *verySmall;

Texture waveNumberText;
Texture waveNumberTitle;
Texture comboNumberText;
Texture playerXPText;
Texture fpsText;

UI_Button* currentButton = nullptr;

Texture logoTexture;

Texture startGameText;

SDL_Renderer* renderer;

Texture selectionTexture;
Texture currentSetupText;

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

SDL_Rect timeToShootBack;
SDL_Rect timeToShoot;

SDL_Rect timeToAbilityBack;
SDL_Rect timeToAbility;

Texture ammoLeftText;
Texture gamePausedText;
Texture xpText;

SDL_Rect upgradeHealth1;
SDL_Rect upgradeHealth2;
SDL_Rect upgradeHealth3;
SDL_Rect upgradeShield1;
SDL_Rect upgradeShield2;

UI_Menu mainMenu(4);
UI_Menu levelSelect(2);
UI_Menu pauseMenu(3);
UI_Menu weaponUpgradeMenu(36);
UI_Menu abilityUpgradeMenu(36);

Player* player;

void UI_init(SDL_Renderer* _renderer, State& state, Player* _player) {
    counter = TTF_OpenFont("resources/sans.ttf",scale(18));
    small = TTF_OpenFont("resources/sans.ttf",scale(12));
    verySmall = TTF_OpenFont("resources/sans.ttf",scale(10));
    title = TTF_OpenFont("resources/sans.ttf",scale(34));

    renderer = _renderer;

    player = _player;

    waveNumberText.setup(renderer);
    waveNumberTitle.setup(renderer);
    comboNumberText.setup(renderer);
    playerXPText.setup(renderer);
    fpsText.setup(renderer);

    startGameText.setup(renderer);
    initPlayerUI();

    initMenus(state);

}

void UI_close() {
    TTF_CloseFont(counter);
    TTF_CloseFont(small);
    TTF_CloseFont(verySmall);
    TTF_CloseFont(title);
}

void updateInGameText(int playerCombo, int wave, int xp) {
    comboNumberText.loadFromRenderedText("Combo: " + std::to_string(playerCombo), white, counter);
    waveNumberText.loadFromRenderedText("Wave: " + std::to_string(wave), white, counter);
    waveNumberTitle.loadFromRenderedText("Wave " + std::to_string(wave) + " Start!", white, title);
    playerXPText.loadFromRenderedText("XP: " + std::to_string(xp), white, counter);

}

void renderInGameText(bool developerMode, float lastFPS,bool waveStarted) {
    waveNumberText.render(scale(10),scale(5));
    comboNumberText.render(scale(10),scale(30));
    playerXPText.render(scale(10),scale(55));
    if(developerMode) {
        fpsText.loadFromRenderedText("FPS: " + std::to_string(lastFPS), white, counter);
        fpsText.render(scale(10),scale(80));
    }
    if(!waveStarted) {
        waveNumberTitle.render((WINDOW_WIDTH-waveNumberTitle.getWidth())/2,scale(200));
    }
}

//Button Action Functions
void showLevelSelect(State& state, int attr1, int attr2) {
    state.menu = level;
    currentButton = levelSelect.loadMenu();
}

void quitToDesktop(State& state, int attr1, int attr2) {
    state.quit = true;
}

void selectLevel1(State& state, int attr1, int attr2) {
    state.level = 1;
    state.menu = notInMenu;
    state.started = true;
}

void selectLevel2(State& state, int attr1, int attr2) {
    state.level = 2;
    state.menu = notInMenu;
    state.started = true;
}

void unpause(State& state, int attr1, int attr2) {
    state.menu = notInMenu;
}

void quitToMenu(State& state, int attr1, int attr2) {
    state.started = false;
    state.menu = head;
    currentButton = mainMenu.loadMenu();
}

void noAction(State& state, int attr1, int attr2) {}

void selectWeapon(State& state, int attr1, int attr2) {
   int weaponLevel;
    switch(attr1) {
        case 1:
            weaponLevel = state.currentRifleLevel;
            break;
        case 2:
            weaponLevel = state.currentShotgunLevel;
            break;
        case 3:
            weaponLevel = state.currentKnifeLevel;
            break;
        case 4:
            weaponLevel = state.currentLaserPistolLevel;
            break;
        default:
            weaponLevel = state.currentRevolverLevel;
            break;
    }
    if(weaponLevel > 0) {
        if(state.weapon1 == attr1) {
            if(state.weapon2 != -1) {
                state.weapon1 = state.weapon2;
                state.weapon2 = -1;
            } else {
                SDL_Log("Must always have 1 weapon!");
            }
        } else if(state.weapon2 == attr1) {
            state.weapon2 = -1;
        } else if(state.weapon1 == -1) {
            state.weapon1 = attr1;
        } else if(state.weapon2 == -1) {
            state.weapon2 = attr1;
        } else {
            //TODO: Add way to display errors to player
            SDL_Log("No weapon slot free");
        }
        loadUpgradeMenu(state);
    }
}

void upgradeWeapon(State& state, int attr1, int attr2) {
    if(player->getXP() >= state.weaponProperties[attr1][attr2][0]) {
        bool upgraded = false;
        switch(attr1) {
        case 1: {
            if(attr2 == state.currentRifleLevel) {
                state.currentRifleLevel++;
                upgraded = true;
            }
        } break;
        case 2: {
            if(attr2 == state.currentShotgunLevel) {
                state.currentShotgunLevel++;
                upgraded = true;
            }
        } break;
        case 3: {
            if(attr2 == state.currentKnifeLevel) {
                state.currentKnifeLevel++;
                upgraded = true;
            }
        } break;
        case 4: {
            if(attr2 == state.currentLaserPistolLevel) {
                state.currentLaserPistolLevel++;
                upgraded = true;
            }
        } break;
        default: {
            if(attr2 == state.currentRevolverLevel) {
                state.currentRevolverLevel++;
                upgraded = true;
            }
        } break;
        }
        if(upgraded) {
            player->changeXP(-state.weaponProperties[attr1][attr2][0]);
            loadUpgradeMenu(state);
        }
    }
}

void selectAbility(State& state, int attr1, int attr2) {
    int abilityLevel;
    Ability ability;
    switch(attr1) {
        case 1:
            ability = teleport;
            abilityLevel = state.abilityLevels[teleport];
            break;
        case 2:
            ability = c4;
            abilityLevel = state.abilityLevels[c4];
            break;
        case 3:
            ability = grenade;
            abilityLevel = state.abilityLevels[grenade];
            break;
        default:
            ability = bounce;
            abilityLevel = state.abilityLevels[bounce];
            break;
    }
    if(abilityLevel > 0) {
        player->setAbility(ability);
        loadUpgradeMenu(state);
    }
}

void upgradeAbility(State& state, int attr1, int attr2) {
    if(player->getXP() >= state.abilityProperties[attr1][attr2][0]) {
        bool upgraded = false;
        switch(attr1) {
        case 1: {
            if(attr2 == state.abilityLevels[teleport]) {
                state.abilityLevels[teleport]++;
                upgraded = true;
            }
        } break;
        case 2: {
            if(attr2 == state.abilityLevels[c4]) {
                state.abilityLevels[c4]++;
                upgraded = true;
            }
        } break;
        case 3: {
            if(attr2 == state.abilityLevels[grenade]) {
                state.abilityLevels[grenade]++;
                upgraded = true;
            }
        } break;
        default: {
            if(attr2 == state.abilityLevels[bounce]) {
                state.abilityLevels[bounce]++;
                upgraded = true;
            }
        } break;
        }
        if(upgraded) {
            player->changeXP(-state.abilityProperties[attr1][attr2][0]);
            loadUpgradeMenu(state);
        }
    }
}

void fullHealth(State& state, int attr1, int attr2) {
    if(player->getXP() >= 15) {
        player->changeXP(-15);
        player->setHP(3);
        loadUpgradeMenu(state);
    }
}

void fullShield(State& state, int attr1, int attr2) {
    if(player->getXP() >= 15) {
        player->changeXP(-15);
        player->setShield(2);
        loadUpgradeMenu(state);
    }
}

void closeUpgradeMenu(State& state, int attr1, int attr2) {
    if(currentButton != nullptr) {
        currentButton->deselect();
        currentButton = nullptr;
    }
    state.menu = notInMenu;
    state.resetShooting = true;
}

void showWeaponMenu(State& state, int attr1, int attr2) {
    state.menu = upgrade;
    currentButton = &(*weaponUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

void showAbilityMenu(State& state, int attr1, int attr2) {
    state.menu = abilityUpgrade;
    currentButton = &(*abilityUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

//Menu Functions
void initMenus(State& state) {
    const int centeredX = (WINDOW_WIDTH-UI_Button::width)/2;

    mainMenu.addRenderer(renderer);
    const int arcadeModeButton = mainMenu.addButton(centeredX,scale(215),"Arcade Mode",&white, counter,-1,-1,-1,-1,&showLevelSelect,state);
    const int storyModeButton = mainMenu.addButton(centeredX,scale(280),"Story Mode",&white, counter,arcadeModeButton,-1,-1,-1,&noAction, state);
    const int settingsButton = mainMenu.addButton(centeredX,scale(345),"Settings",&white, counter,storyModeButton,-1,-1,-1, &noAction, state);
    mainMenu.addButton(centeredX,scale(410),"Quit To Desktop",&white, counter,settingsButton,-1,-1,-1,&quitToDesktop,state);
    logoTexture.setup(scale(454),scale(92),renderer);
    logoTexture.loadFromFile("logo.png");
    mainMenu.addTitle((WINDOW_WIDTH-scale(454))/2,scale(100), logoTexture);

    levelSelect.addRenderer(renderer);
    const int level1Button = levelSelect.addButton(centeredX,scale(225),"Level 1",&white, counter,-1,-1,-1,-1, &selectLevel1, state);
    levelSelect.addButton(centeredX,scale(290),"Level 2",&white, counter,level1Button,-1,-1,-1, &selectLevel2, state);
    levelSelect.addTitle((WINDOW_WIDTH-scale(454))/2,scale(100), logoTexture);

    pauseMenu.addRenderer(renderer);
    const int resumeButton = pauseMenu.addButton(centeredX,scale(215),"Resume Game", &white, counter, -1,-1,-1,-1, &unpause, state);
    const int quitToMenuButton = pauseMenu.addButton(centeredX,scale(280),"Quit To Menu", &white, counter,resumeButton,-1,-1,-1,&quitToMenu, state);
    pauseMenu.addButton(centeredX,scale(345), "Quit To Desktop", &white, counter, quitToMenuButton,-1,-1,-1,&quitToDesktop, state);
    gamePausedText.setup(renderer);
    gamePausedText.loadFromRenderedText("Game Paused", white, title);
    pauseMenu.addTitle((WINDOW_WIDTH-gamePausedText.getWidth())/2,scale(100),gamePausedText);

    initWeaponUpgradeMenu(state);
    initAbilityUpgradeMenu(state);
}

std::string removeTrailingZeros(double i) {
    std::string result = std::to_string(i);
    result.erase(result.find_last_not_of('0') + 1, std::string::npos); // Remove trailing zeros
    if (result.back() == '.') {
        result.pop_back();  // Remove the decimal point if it's at the end
    }
    return result;
}

void genericUpgradeMenuLayout(State& state, UI_Menu* menu) {
    menu->addRenderer(renderer);
    menu->addButton(scale(37),scale(100),"Max HP", &white,small,-1,-1,-1,-1, &fullHealth, state,1);
    menu->getButtons()->back().setupHover(1);
    menu->getButtons()->back().addLine("Cost: ", removeTrailingZeros(15), verySmall, white);

    menu->addButton(scale(37),scale(140),"Max Shield", &white,small,0,-1,-1,-1, &fullShield, state,1);
    menu->getButtons()->back().setupHover(1);
    menu->getButtons()->back().addLine("Cost: ", removeTrailingZeros(15), verySmall, white);

    menu->addButton(scale(37),scale(180),"Accept Changes", &white,small,1,-1,-1,-1, &closeUpgradeMenu, state,1);

    menu->addButton(scale(160+128),scale(12),"Weapons", &white,small,-1,-1,-1,-1, &showWeaponMenu, state,1);
    menu->addButton(scale(160+256),scale(12),"Abilities", &white,small,-1,-1,-1,-1, &showAbilityMenu, state,1);
    menu->addButton(scale(160+384),scale(12),"Player", &white,small,-1,-1,-1,-1, &noAction, state,1);
}

void initWeaponUpgradeMenu(State& state) {

    genericUpgradeMenuLayout(state,&weaponUpgradeMenu);

    for(int i = 0; i < state.numberOfWeapons; i++) {
        int start;
        if(i==0) {
            start = 2;
        } else {
            start = 0;
        }
        weaponUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16)," ", &white,small,-1,-1,start+i*6,-1, &selectWeapon, state,3,i);
        for(int j = 0; j < 5; j++) {
            std::string path;
            if(i == 0) {
                path = "upgrade-menu/upgrade-revolver.png";
            } else if(i == 1) {
                path = "upgrade-menu/upgrade-rifle.png";
            } else if(i == 2) {
                path = "upgrade-menu/upgrade-shotgun.png";
            } else if(i == 3) {
                path = "upgrade-menu/upgrade-knife.png";
            } else if(i == 4) {
                path = "upgrade-menu/upgrade-laser-pistol.png";
            }

            if(i == 0) {
                weaponUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,6+j,2,-1, &upgradeWeapon, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
            } else {
                weaponUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,6+i*6+j,7+(i-1)*6+j,-1, &upgradeWeapon, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
            }

            weaponUpgradeMenu.getButtons()->back().setupHover(6);
            weaponUpgradeMenu.getButtons()->back().addLine("Cost: ", removeTrailingZeros(state.weaponProperties[i][j][0]), verySmall, white);
            if(i == 0 || i == 1) {
                weaponUpgradeMenu.getButtons()->back().addLine("Clip Size: ", removeTrailingZeros(state.weaponProperties[i][j][1]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Reload Speed : ", removeTrailingZeros(state.weaponProperties[i][j][2]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Bullet Durability: ", removeTrailingZeros(state.weaponProperties[i][j][3]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Damage: ", removeTrailingZeros(state.weaponProperties[i][j][4]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Strength: ", removeTrailingZeros(state.weaponProperties[i][j][5]), verySmall, white);
            } else if(i== 2) {
                weaponUpgradeMenu.getButtons()->back().addLine("Clip Size: ", removeTrailingZeros(state.weaponProperties[i][j][1]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Reload Speed : ", removeTrailingZeros(state.weaponProperties[i][j][2]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Bullet Durability: ", removeTrailingZeros(state.weaponProperties[i][j][3]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Damage: ", removeTrailingZeros(state.weaponProperties[i][j][4]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("# of Bullets: ", removeTrailingZeros(state.weaponProperties[i][j][5]), verySmall, white);
            } else if(i == 3) {
                weaponUpgradeMenu.getButtons()->back().addLine("Damage: ", removeTrailingZeros(state.weaponProperties[i][j][1]), verySmall, white);
            } else if(i == 4) {
                weaponUpgradeMenu.getButtons()->back().addLine("Cool Fire Rate: ", removeTrailingZeros(state.weaponProperties[i][j][1]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Heat Buffer: ", removeTrailingZeros(state.weaponProperties[i][j][2]), verySmall, white);
                weaponUpgradeMenu.getButtons()->back().addLine("Cool Off Time: ", removeTrailingZeros(state.weaponProperties[i][j][3]), verySmall, white);
            }
        }
    }
    (*weaponUpgradeMenu.getButtons())[2].linkButtons(&(*weaponUpgradeMenu.getButtons())[1],&(*weaponUpgradeMenu.getButtons())[6],nullptr,&(*weaponUpgradeMenu.getButtons())[6]);

    xpText.setup(renderer);
    xpText.loadFromRenderedText("XP: 0", white, counter);

    upgradeHealth1 = {scale(37),scale(72),scale(15),scale(15)};
    upgradeHealth2 = {scale(57),scale(72),scale(15),scale(15)};
    upgradeHealth3 = {scale(77),scale(72),scale(15),scale(15)};
    upgradeShield1 = {scale(97),scale(72),scale(15),scale(15)};
    upgradeShield2 = {scale(117),scale(72),scale(15),scale(15)};
}

void loadUpgradeMenu(State& state) {
    for(auto& button : *weaponUpgradeMenu.getButtons()) {
        button.deactivate();
    }
    if(state.weapon1 != -1)  {
        (*weaponUpgradeMenu.getButtons())[6+state.weapon1*6].activate();
    }
    if(state.weapon2 != -1) {
        (*weaponUpgradeMenu.getButtons())[6+state.weapon2*6].activate();
    }

    for(int i = 0; i < 5;i++) {
        (*weaponUpgradeMenu.getButtons())[7 + i].enable();
        (*weaponUpgradeMenu.getButtons())[13 + i].enable();
        (*weaponUpgradeMenu.getButtons())[19 + i].enable();
        (*weaponUpgradeMenu.getButtons())[25 + i].enable();
        (*weaponUpgradeMenu.getButtons())[31 + i].enable();
        if(i>state.currentRevolverLevel) {
            (*weaponUpgradeMenu.getButtons())[7 + i].disable();
        } else if(i != state.currentRevolverLevel) {
            (*weaponUpgradeMenu.getButtons())[7 + i].activate();
        }
        if(i>state.currentRifleLevel) {
            (*weaponUpgradeMenu.getButtons())[13 + i].disable();
        } else if(i != state.currentRifleLevel) {
            (*weaponUpgradeMenu.getButtons())[13 + i].activate();
        }
        if(i>state.currentShotgunLevel) {
            (*weaponUpgradeMenu.getButtons())[19 + i].disable();
        } else if(i != state.currentShotgunLevel) {
            (*weaponUpgradeMenu.getButtons())[19 + i].activate();
        }
        if(i>state.currentKnifeLevel) {
            (*weaponUpgradeMenu.getButtons())[25 + i].disable();
        } else if(i != state.currentKnifeLevel) {
            (*weaponUpgradeMenu.getButtons())[25 + i].activate();
        }
        if(i>state.currentLaserPistolLevel) {
            (*weaponUpgradeMenu.getButtons())[31 + i].disable();
        } else if(i != state.currentLaserPistolLevel) {
            (*weaponUpgradeMenu.getButtons())[31 + i].activate();
        }
    }

    for(auto& button : *abilityUpgradeMenu.getButtons()) {
        button.deactivate();
    }

    if(player->getAbility() != none)  {
        (*abilityUpgradeMenu.getButtons())[6+player->getAbility()*6].activate();
    }

    for(int i = 0; i < 5;i++) {
        (*abilityUpgradeMenu.getButtons())[7 + i].enable();
        (*abilityUpgradeMenu.getButtons())[13 + i].enable();
        (*abilityUpgradeMenu.getButtons())[19 + i].enable();
        (*abilityUpgradeMenu.getButtons())[25 + i].enable();
        (*abilityUpgradeMenu.getButtons())[31 + i].enable();
        if(i>state.abilityLevels[bounce]) {
            (*abilityUpgradeMenu.getButtons())[7 + i].disable();
        } else if(i != state.abilityLevels[bounce]) {
            (*abilityUpgradeMenu.getButtons())[7 + i].activate();
        }
        if(i>state.abilityLevels[teleport]) {
            (*abilityUpgradeMenu.getButtons())[13 + i].disable();
        } else if(i != state.abilityLevels[teleport]) {
            (*abilityUpgradeMenu.getButtons())[13 + i].activate();
        }
        if(i>state.abilityLevels[c4]) {
            (*abilityUpgradeMenu.getButtons())[19 + i].disable();
        } else if(i != state.abilityLevels[c4]) {
            (*abilityUpgradeMenu.getButtons())[19 + i].activate();
        }
        if(i>state.abilityLevels[grenade]) {
            (*abilityUpgradeMenu.getButtons())[25 + i].disable();
        } else if(i != state.abilityLevels[grenade]) {
            (*abilityUpgradeMenu.getButtons())[25 + i].activate();
        }
    }

    xpText.loadFromRenderedText("Current XP: " + std::to_string(player->getXP()), white, counter);
}

void launchUpgradeMenu() {
    currentButton = &(*weaponUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

void initAbilityUpgradeMenu(State& state) {
    genericUpgradeMenuLayout(state,&abilityUpgradeMenu);

    for(int i = 0; i < state.numberOfAbilities; i++) {
        int start;
        if(i==0) {
            start = 2;
        } else {
            start = 0;
        }
        abilityUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16)," ", &white,small,-1,-1,start+i*6,-1, &selectAbility, state,3,i);
        for(int j = 0; j < 5; j++) {
            std::string path;
            if(i == 0) {
                path = "upgrade-menu/upgrade-bounce.png";
            } else if(i == 1) {
                path = "upgrade-menu/upgrade-teleport.png";
            } else if(i == 2) {
                path = "upgrade-menu/upgrade-c4.png";
            } else if(i == 3) {
                path = "upgrade-menu/upgrade-grenade.png";
            }

            if(i == 0) {
                abilityUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,6+j,2,-1, &upgradeAbility, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
            } else {
                abilityUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,6+i*6+j,7+(i-1)*6+j,-1, &upgradeAbility, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
            }
            abilityUpgradeMenu.getButtons()->back().setupHover(3);
            abilityUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.abilityProperties[i][j][0]), verySmall, white);
            abilityUpgradeMenu.getButtons()->back().addLine("Refresh: ",removeTrailingZeros(state.abilityProperties[i][j][1]), verySmall, white);

            if(i == 1) {
                abilityUpgradeMenu.getButtons()->back().addLine("Duration: ",removeTrailingZeros(state.abilityProperties[i][j][2]), verySmall, white);
            } else if(i == 2) {
                abilityUpgradeMenu.getButtons()->back().addLine("Damage: ",removeTrailingZeros(state.abilityProperties[i][j][2]), verySmall, white);
            } else if(i == 3) {
                abilityUpgradeMenu.getButtons()->back().addLine("Damage: ",removeTrailingZeros(state.abilityProperties[i][j][2]), verySmall, white);
            }

        }
    }
    (*abilityUpgradeMenu.getButtons())[2].linkButtons(&(*abilityUpgradeMenu.getButtons())[1],&(*abilityUpgradeMenu.getButtons())[6],nullptr,&(*abilityUpgradeMenu.getButtons())[6]);

}

UI_Menu* getCurrentMenu(State& state) {
    switch(state.menu) {
        case head:
            return &mainMenu;
        case level:
            return &levelSelect;
        case pause:
            return &pauseMenu;
        case upgrade:
            return &weaponUpgradeMenu;
        case abilityUpgrade:
            return &abilityUpgradeMenu;
        default:
            return nullptr;
    }
}

void renderUpgradeMenu(State& state) {
    xpText.render(scale(37),scale(37));
    switch(player->getShield()) {
        case 2: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer,&upgradeShield1);
            SDL_RenderFillRect(renderer,&upgradeShield2);
        } break;
        case 1: {
            SDL_SetRenderDrawColor(renderer, 183, 201, 226, 255);
            SDL_RenderFillRect(renderer,&upgradeShield1);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer,&upgradeShield2);
        } break;
        default: {
            SDL_SetRenderDrawColor(renderer, 183, 201, 226, 255);
            SDL_RenderFillRect(renderer,&upgradeShield1);
            SDL_RenderFillRect(renderer,&upgradeShield2);
        } break;
    }

    switch(player->getHP()) {
        case 3: {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&upgradeHealth1);
            SDL_RenderFillRect(renderer,&upgradeHealth2);
            SDL_RenderFillRect(renderer,&upgradeHealth3);
        } break;
        case 2: {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&upgradeHealth1);
            SDL_RenderFillRect(renderer,&upgradeHealth2);
            SDL_SetRenderDrawColor(renderer, 170, 104, 95, 255);
            SDL_RenderFillRect(renderer,&upgradeHealth3);
        } break;
        case 1: {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&upgradeHealth1);
            SDL_SetRenderDrawColor(renderer, 170, 104, 95, 255);
            SDL_RenderFillRect(renderer,&upgradeHealth2);
            SDL_RenderFillRect(renderer,&upgradeHealth3);
        } break;
    default:
        break;
    }


}

void renderMenu(State& state) {
  if (UI_Menu *currentMenu = getCurrentMenu(state); currentMenu != nullptr) {
      currentMenu->render();
  }
    if(state.menu == upgrade || state.menu == abilityUpgrade) {
        renderUpgradeMenu(state);
    }
}


void mouseMove(State& state) {
    int x, y;
    SDL_GetMouseState( &x, &y );

    if(!state.controller) {
        if(currentButton != nullptr) {
            currentButton->deselect();
            currentButton = nullptr;
        }
    }

    if (UI_Menu *currentMenu = getCurrentMenu(state); currentMenu != nullptr) {
        std::vector<UI_Button>* menuButtons = currentMenu->getButtons();
        for(auto it = menuButtons->begin(); it != menuButtons->end(); ++it) {
            if(it->mouseEvent(x,y) && !it->isDisabled()) {
                if(currentButton != nullptr) {
                    currentButton->deselect();
                }
                currentButton = &*it;
                currentButton->select();
            }
        }
    }
}

void menuSelect(State& state) {
    if(UI_Menu *currentMenu = getCurrentMenu(state); currentMenu != nullptr) {
        std::vector<UI_Button>* menuButtons = currentMenu->getButtons();
        for(auto & menuButton : *menuButtons) {
            if(menuButton.isSelected()) {
                menuButton.click();
            }
        }
    }
}

void controllerEvent(State& state, MENU_CONTROL control) {
    if(UI_Menu *currentMenu = getCurrentMenu(state); currentMenu != nullptr) {
        switch(control) {
            case MENU_CONTROL::connect:
                currentButton = currentMenu->loadMenu();
                break;
            case MENU_CONTROL::disconnect: {
                currentButton->deselect();
                currentButton = nullptr;
            } break;
            case MENU_CONTROL::select:
                menuSelect(state);
                break;
            default: {
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
            } break;
        }
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

    if(player->getAbility() != none) {
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

    if(player->getAbility() == c4) {
        for(int i = 0; i < 3; i++) {
            if(player->getC4Left()>i) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }
            SDL_Rect tempRect;
            tempRect.x = WINDOW_WIDTH-scale(30)-scale(20*i);
            tempRect.y = WINDOW_HEIGHT-scale(100);
            tempRect.w = scale(15);
            tempRect.h = scale(15);
            SDL_RenderFillRect(renderer,&tempRect);
        }
    }

}

