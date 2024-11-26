#include "../includes/UI.h"
#include "../includes/GlobalConstants.h"
#include "../includes/State.h"

#include <SDL_ttf.h>
#include <string>

#include "../includes/Enemy.h"
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

SDL_Rect healthBackRect;
SDL_Rect healthRect;
SDL_Rect shieldBackRect;
SDL_Rect shieldRect;

Texture ammoLeftText;
Texture gamePausedText;
Texture xpText;
Texture rbText;
Texture lbText;

Texture roborTexture;
Texture robortoTexture;
Texture robroTexture;
Texture romoTexture;
Texture rooTexture;

Texture roborText;
Texture robortoText;
Texture robroText;
Texture romoText;
Texture rooText;

SDL_Rect robortoSource;
SDL_Rect rooSource;

SDL_Rect upgradeHealth;
SDL_Rect upgradeHealthBack;
SDL_Rect upgradeShield;
SDL_Rect upgradeShieldBack;

UI_Menu mainMenu(4);
UI_Menu levelSelect(3);
UI_Menu pauseMenu(3);
UI_Menu weaponUpgradeMenu(36);
UI_Menu abilityUpgradeMenu(36);
UI_Menu playerUpgradeMenu(36);

Sound buttonSound;

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

void resetMenus(State& state) {
    weaponUpgradeMenu.reset();
    abilityUpgradeMenu.reset();
    playerUpgradeMenu.reset();

    initWeaponUpgradeMenu(state);
    initAbilityUpgradeMenu(state);
    initPlayerUpgradeMenu(state);
}

void purchaseUpgrade(State& state) {
    state.upgradeIncreaseFactor += 5;
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

void selectLevel3(State& state, int attr1, int attr2) {
    state.level = 3;
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
            weaponLevel = state.weaponLevels[rifle];
            break;
        case 2:
            weaponLevel = state.weaponLevels[shotgun];
            break;
        case 3:
            weaponLevel = state.weaponLevels[knife];
            break;
        case 4:
            weaponLevel = state.weaponLevels[laserPistol];
            break;
        default:
            weaponLevel = state.weaponLevels[revolver];
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
    if(player->getXP() >= state.weaponProperties[attr1][attr2][0] + state.upgradeIncreaseFactor) {
        bool upgraded = false;
        switch(attr1) {
        case 1: {
            if(attr2 == state.weaponLevels[rifle]) {
                state.weaponLevels[rifle]++;
                upgraded = true;
            }
        } break;
        case 2: {
            if(attr2 == state.weaponLevels[shotgun]) {
                state.weaponLevels[shotgun]++;
                upgraded = true;
            }
        } break;
        case 3: {
            if(attr2 == state.weaponLevels[knife]) {
                state.weaponLevels[knife]++;
                upgraded = true;
            }
        } break;
        case 4: {
            if(attr2 == state.weaponLevels[laserPistol]) {
                state.weaponLevels[laserPistol]++;
                SDL_Log("Laser UPgrade: %i", state.weaponLevels[laserPistol]);
                upgraded = true;
            }
        } break;
        default: {
            if(attr2 == state.weaponLevels[revolver]) {
                state.weaponLevels[revolver]++;
                upgraded = true;
            }
        } break;
        }
        if(upgraded) {
            player->changeXP(-(state.weaponProperties[attr1][attr2][0]+ state.upgradeIncreaseFactor));
            purchaseUpgrade(state);
            resetMenus(state);
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
    if(player->getXP() >= state.abilityProperties[attr1][attr2][0]+ state.upgradeIncreaseFactor) {
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
            player->changeXP(-(state.abilityProperties[attr1][attr2][0] + state.upgradeIncreaseFactor));
            purchaseUpgrade(state);
            resetMenus(state);
            loadUpgradeMenu(state);
        }
    }
}

void upgradePlayer(State& state, int attr1, int attr2) {
    if(player->getXP() >= state.playerProperties[attr1][attr2][0] + state.upgradeIncreaseFactor) {
        bool upgraded = false;
        switch(attr1) {
        case 1: {
            if(attr2 == state.playerLevels[shield]) {
                state.playerLevels[shield]++;
                upgraded = true;
            }
        } break;
        case 2: {
            if(attr2 == state.playerLevels[speed]) {
                state.playerLevels[speed]++;
                upgraded = true;
            }
        } break;
        case 3: {
            if(attr2 == state.playerLevels[dodge]) {
                state.playerLevels[dodge]++;
                upgraded = true;
            }
        } break;
        default: {
            if(attr2 == state.playerLevels[armor]) {
                state.playerLevels[armor]++;
                upgraded = true;
            }
        } break;
        }
        if(upgraded) {
            player->changeXP(-(state.playerProperties[attr1][attr2][0] + state.upgradeIncreaseFactor));
            purchaseUpgrade(state);
            resetMenus(state);
            loadUpgradeMenu(state);
        }
    }
}

void fullHealth(State& state, int attr1, int attr2) {
    if(player->getXP() >= 15 + state.upgradeIncreaseFactor) {
        player->changeXP(-(15 + state.upgradeIncreaseFactor));
        player->fullHealth();
        purchaseUpgrade(state);
        resetMenus(state);
        loadUpgradeMenu(state);
    }
}

void fullShield(State& state, int attr1, int attr2) {
    if(player->getXP() >= 15 + state.upgradeIncreaseFactor) {
        player->changeXP(-(15+ state.upgradeIncreaseFactor));
        player->fillShield();
        purchaseUpgrade(state);
        resetMenus(state);
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

void showWeaponUpgradeMenu(State& state, int attr1, int attr2) {
    (*weaponUpgradeMenu.getButtons())[3].activate();
    (*abilityUpgradeMenu.getButtons())[4].deactivate();
    (*playerUpgradeMenu.getButtons())[5].deactivate();
    closeUpgradeMenu(state,attr1,attr2);
    state.menu = weaponUpgrade;
    currentButton = &(*weaponUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

void showAbilityUpgradeMenu(State& state, int attr1, int attr2) {
    (*weaponUpgradeMenu.getButtons())[3].deactivate();
    (*abilityUpgradeMenu.getButtons())[4].activate();
    (*playerUpgradeMenu.getButtons())[5].deactivate();
    closeUpgradeMenu(state,attr1,attr2);
    state.menu = abilityUpgrade;
    currentButton = &(*abilityUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

void showPlayerUpgradeMenu(State& state, int attr1, int attr2) {
    (*weaponUpgradeMenu.getButtons())[3].deactivate();
    (*abilityUpgradeMenu.getButtons())[4].deactivate();
    (*playerUpgradeMenu.getButtons())[5].activate();
    closeUpgradeMenu(state,attr1,attr2);
    state.menu = playerUpgrade;
    currentButton = &(*playerUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}

//Menu Functions
void initMenus(State& state) {
    const int centeredX = (WINDOW_WIDTH-UI_Button::width)/2;

    buttonSound.init("resources/sounds/buttonClick.wav", 0,-1);

    mainMenu.setup(renderer, &buttonSound);
    const int arcadeModeButton = mainMenu.addButton(centeredX,scale(215),"Arcade Mode",&white, counter,-1,-1,-1,-1,&showLevelSelect,state);
    const int storyModeButton = mainMenu.addButton(centeredX,scale(280),"Campaign Mode",&white, counter,arcadeModeButton,-1,-1,-1,&noAction, state);
    const int settingsButton = mainMenu.addButton(centeredX,scale(345),"Settings",&white, counter,storyModeButton,-1,-1,-1, &noAction, state);
    mainMenu.addButton(centeredX,scale(410),"Quit To Desktop",&white, counter,settingsButton,-1,-1,-1,&quitToDesktop,state);
    logoTexture.setup(scale(454),scale(92),renderer);
    logoTexture.loadFromFile("logo.png");
    mainMenu.addTitle((WINDOW_WIDTH-scale(454))/2,scale(100), logoTexture);

    levelSelect.setup(renderer, &buttonSound);
    const int level1Button = levelSelect.addButton(centeredX,scale(225),"Level 1",&white, counter,-1,-1,-1,-1, &selectLevel1, state);
    //const int level2Button = levelSelect.addButton(centeredX,scale(290),"Level 2",&white, counter,level1Button,-1,-1,-1, &selectLevel3, state);
    //levelSelect.addButton(centeredX,scale(355),"Level 3",&white, counter,level2Button,-1,-1,-1, &selectLevel3, state);
    levelSelect.addTitle((WINDOW_WIDTH-scale(454))/2,scale(100), logoTexture);

    pauseMenu.setup(renderer, &buttonSound);
    const int resumeButton = pauseMenu.addButton(centeredX,scale(215),"Resume Game", &white, counter, -1,-1,-1,-1, &unpause, state);
    const int quitToMenuButton = pauseMenu.addButton(centeredX,scale(280),"Quit To Menu", &white, counter,resumeButton,-1,-1,-1,&quitToMenu, state);
    pauseMenu.addButton(centeredX,scale(345), "Quit To Desktop", &white, counter, quitToMenuButton,-1,-1,-1,&quitToDesktop, state);
    gamePausedText.setup(renderer);
    gamePausedText.loadFromRenderedText("Game Paused", white, title);
    pauseMenu.addTitle((WINDOW_WIDTH-gamePausedText.getWidth())/2,scale(100),gamePausedText);

    rbText.setup(scale(40),scale(40),renderer);
    lbText.setup(scale(40),scale(40),renderer);
    rbText.loadFromFile("upgrade-menu/rb.png");
    lbText.loadFromFile("upgrade-menu/lb.png");

    roborTexture.setup(scale(24),scale(50),renderer);
    roborTexture.loadFromFile("robor.png");

    robortoTexture.setup(scale(28),scale(50),renderer);
    robortoTexture.loadFromFile("roborto.png");

    robroTexture.setup(scale(19),scale(50),renderer);
    robroTexture.loadFromFile("robro.png");

    romoTexture.setup(scale(31),scale(20),renderer);
    romoTexture.loadFromFile("romo.png");

    rooTexture.setup(scale(36),scale(50),renderer);
    rooTexture.loadFromFile("roo.png");

    roborText.setup(renderer);
    robortoText.setup(renderer);
    robroText.setup(renderer);
    romoText.setup(renderer);
    rooText.setup(renderer);

    robortoSource = {18,0,18,32};
    rooSource = {92,0,23,32};

    initWeaponUpgradeMenu(state);
    initAbilityUpgradeMenu(state);
    initPlayerUpgradeMenu(state);
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
    menu->setup(renderer, &buttonSound);
    menu->addButton(scale(37),scale(100),"Max HP", &white,small,-1,-1,-1,-1, &fullHealth, state,1);
    menu->getButtons()->back().setupHover(1);
    menu->getButtons()->back().addLine("Cost: ", removeTrailingZeros(15 + state.upgradeIncreaseFactor), verySmall, white);

    menu->addButton(scale(37),scale(140),"Max Shield", &white,small,0,-1,-1,-1, &fullShield, state,1);
    menu->getButtons()->back().setupHover(1);
    menu->getButtons()->back().addLine("Cost: ", removeTrailingZeros(15 + state.upgradeIncreaseFactor), verySmall, white);

    menu->addButton(scale(37),scale(180),"Close Menu", &white,small,1,-1,-1,-1, &closeUpgradeMenu, state,1);

    menu->addButton(scale(160+128),scale(12),"Weapons", &white,small,-1,-1,-1,-1, &showWeaponUpgradeMenu, state,1);
    menu->addButton(scale(160+256),scale(12),"Abilities", &white,small,-1,-1,-1,-1, &showAbilityUpgradeMenu, state,1);
    menu->addButton(scale(160+384),scale(12),"Player", &white,small,-1,-1,-1,-1, &showPlayerUpgradeMenu, state,1);
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
            if(state.weaponLevels[i] < j+1) {
                weaponUpgradeMenu.getButtons()->back().addLine("Cost: ", removeTrailingZeros(state.weaponProperties[i][j][0] + state.upgradeIncreaseFactor), verySmall, white);
            }
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

    upgradeHealth = {scale(50),scale(55),scale(102),scale(15)};
    upgradeHealthBack = {scale(50),scale(55),scale(102),scale(15)};
    upgradeShield = {scale(50),scale(75),scale(102),scale(15)};
    upgradeShieldBack = {scale(50),scale(75),scale(102),scale(15)};
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
            if(state.abilityLevels[i] < j+1) {
                abilityUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.abilityProperties[i][j][0] + state.upgradeIncreaseFactor), verySmall, white);
            }
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

void initPlayerUpgradeMenu(State& state) {
    genericUpgradeMenuLayout(state,&playerUpgradeMenu);

    for(int i = 0; i < state.numberOfPlayerUpgrades; i++) {
        int start;
        if(i==0) {
            start = 2;
        } else {
            start = 0;
        }
        for(int j = 0; j < 5; j++) {
            std::string path;
            if(i == 0) {
                path = "upgrade-menu/upgrade-armor.png";
            } else if(i == 1) {
                path = "upgrade-menu/upgrade-shield.png";
            } else if(i == 2) {
                path = "upgrade-menu/upgrade-speed.png";
            } else if(i == 3) {
                path = "upgrade-menu/upgrade-dodge.png";
            }

            if(i == 0) {
                if(j==0) {
                    playerUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,2,2,-1, &upgradePlayer, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
                } else {
                    playerUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,5+j,2,-1, &upgradePlayer, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
                }
            } else {
                if(j==0) {
                    playerUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,2,6+(i-1)*5+j,-1, &upgradePlayer, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
                } else {
                    playerUpgradeMenu.addButton(scale(200)+scale(100*i),WINDOW_HEIGHT-scale(30+16+60+16) - scale((16+60)*j),path, -1,5+i*5+j,6+(i-1)*5+j,-1, &upgradePlayer, state,2,i,j, "upgrade-menu/upgrade-" + std::to_string(j+1) + ".png");
                }
            }
            playerUpgradeMenu.getButtons()->back().setupHover(3);

            if(i == 0) {
                playerUpgradeMenu.getButtons()->back().addLine("Armor: "," ", verySmall, white);
                if(state.playerLevels[i] < j+1) {
                    playerUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.playerProperties[i][j][0] + state.upgradeIncreaseFactor), verySmall, white);
                }
                playerUpgradeMenu.getButtons()->back().addLine("Damage Reduction: ",removeTrailingZeros(state.playerProperties[i][j][1]) + "%", verySmall, white);
            } else if(i == 1) {
                playerUpgradeMenu.getButtons()->back().addLine("Shield Charge: "," ", verySmall, white);
                if(state.playerLevels[i] < j+1) {
                    playerUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.playerProperties[i][j][0]), verySmall, white);
                }
                playerUpgradeMenu.getButtons()->back().addLine("Multiplier: ",removeTrailingZeros(state.playerProperties[i][j][1]), verySmall, white);
            } else if(i == 2) {
                playerUpgradeMenu.getButtons()->back().addLine("Player Speed: "," ", verySmall, white);
                if(state.playerLevels[i] < j+1) {
                    playerUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.playerProperties[i][j][0]), verySmall, white);
                }
                playerUpgradeMenu.getButtons()->back().addLine("Speed Increase: ",removeTrailingZeros(state.playerProperties[i][j][1])+ "%", verySmall, white);
            } else if(i == 3) {
                playerUpgradeMenu.getButtons()->back().addLine("Dodge: "," ", verySmall, white);
                if(state.playerLevels[i] < j+1) {
                    playerUpgradeMenu.getButtons()->back().addLine("Cost: ",removeTrailingZeros(state.playerProperties[i][j][0]), verySmall, white);
                }
                playerUpgradeMenu.getButtons()->back().addLine("Dodge Chance: ",removeTrailingZeros(state.playerProperties[i][j][1])+ "%", verySmall, white);
            }

        }
    }
    (*playerUpgradeMenu.getButtons())[2].linkButtons(&(*playerUpgradeMenu.getButtons())[1],&(*playerUpgradeMenu.getButtons())[6],nullptr,&(*playerUpgradeMenu.getButtons())[6]);

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
        if(i>state.weaponLevels[revolver]) {
            (*weaponUpgradeMenu.getButtons())[7 + i].disable();
        } else if(i != state.weaponLevels[revolver]) {
            (*weaponUpgradeMenu.getButtons())[7 + i].activate();
        }
        if(i>state.weaponLevels[rifle]) {
            (*weaponUpgradeMenu.getButtons())[13 + i].disable();
        } else if(i != state.weaponLevels[rifle]) {
            (*weaponUpgradeMenu.getButtons())[13 + i].activate();
        }
        if(i>state.weaponLevels[shotgun]) {
            (*weaponUpgradeMenu.getButtons())[19 + i].disable();
        } else if(i != state.weaponLevels[shotgun]) {
            (*weaponUpgradeMenu.getButtons())[19 + i].activate();
        }
        if(i>state.weaponLevels[knife]) {
            (*weaponUpgradeMenu.getButtons())[25 + i].disable();
        } else if(i != state.weaponLevels[knife]) {
            (*weaponUpgradeMenu.getButtons())[25 + i].activate();
        }
        if(i>state.weaponLevels[laserPistol]) {
            (*weaponUpgradeMenu.getButtons())[31 + i].disable();
        } else if(i != state.weaponLevels[laserPistol]) {
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

    for(auto& button : *playerUpgradeMenu.getButtons()) {
        button.deactivate();
    }
    for(int i = 0; i < 5;i++) {
        (*playerUpgradeMenu.getButtons())[6 + i].enable();
        (*playerUpgradeMenu.getButtons())[11 + i].enable();
        (*playerUpgradeMenu.getButtons())[16 + i].enable();
        (*playerUpgradeMenu.getButtons())[21 + i].enable();
        if(i>state.playerLevels[armor]) {
            (*playerUpgradeMenu.getButtons())[6 + i].disable();
        } else if(i != state.playerLevels[armor]) {
            (*playerUpgradeMenu.getButtons())[6 + i].activate();
        }
        if(i>state.playerLevels[shield]) {
            (*playerUpgradeMenu.getButtons())[11 + i].disable();
        } else if(i != state.playerLevels[shield]) {
            (*playerUpgradeMenu.getButtons())[11 + i].activate();
        }
        if(i>state.playerLevels[speed]) {
            (*playerUpgradeMenu.getButtons())[16 + i].disable();
        } else if(i != state.playerLevels[speed]) {
            (*playerUpgradeMenu.getButtons())[16 + i].activate();
        }
        if(i>state.playerLevels[dodge]) {
            (*playerUpgradeMenu.getButtons())[21 + i].disable();
        } else if(i != state.playerLevels[dodge]) {
            (*playerUpgradeMenu.getButtons())[21 + i].activate();
        }
    }

    switch(state.menu) {
        default:
            (*weaponUpgradeMenu.getButtons())[3].activate();
            break;
        case abilityUpgrade:
            (*abilityUpgradeMenu.getButtons())[4].activate();
            break;
        case playerUpgrade:
            (*playerUpgradeMenu.getButtons())[5].activate();
            break;

    }

    xpText.loadFromRenderedText("Current XP: " + std::to_string(player->getXP()), white, counter);
}

void launchUpgradeMenu(State& state) {
    roborText.loadFromRenderedText("x" + std::to_string(state.setEnemies[robor]),white,counter);
    robortoText.loadFromRenderedText("x" + std::to_string(state.setEnemies[roborto]),white,counter);
    robroText.loadFromRenderedText("x" + std::to_string(state.setEnemies[robro]),white,counter);
    romoText.loadFromRenderedText("x" + std::to_string(state.setEnemies[romo]),white,counter);
    rooText.loadFromRenderedText("x" + std::to_string(state.setEnemies[roo]),white,counter);

    resetMenus(state);

    currentButton = &(*weaponUpgradeMenu.getButtons())[2];
    if(currentButton != nullptr) {
        currentButton->select();
    }
}



UI_Menu* getCurrentMenu(State& state) {
    switch(state.menu) {
        case head:
            return &mainMenu;
        case level:
            return &levelSelect;
        case pause:
            return &pauseMenu;
        case weaponUpgrade:
            return &weaponUpgradeMenu;
        case abilityUpgrade:
            return &abilityUpgradeMenu;
        case playerUpgrade:
            return &playerUpgradeMenu;
        default:
            return nullptr;
    }
}

void renderUpgradeMenu(State& state) {
    xpText.render(scale(37),scale(23));
    lbText.render(scale(245),scale(5));
    rbText.render(scale(675),scale(5));

    roborTexture.render(scale(50-roborTexture.getWidth()/2),scale(250));
    robortoTexture.render(scale(120-robortoTexture.getWidth()/2),scale(290),SDL_FLIP_NONE,&robortoSource);
    robroTexture.render(scale(50-robroTexture.getWidth()/2),scale(330));
    romoTexture.render(scale(120-romoTexture.getWidth()/2),scale(400));
    rooTexture.render(scale(50-rooTexture.getWidth()/2),scale(410),SDL_FLIP_NONE,&rooSource);

    roborText.render(scale(50-roborTexture.getWidth()/2)+roborTexture.getWidth()+scale(5),scale(250)+roborTexture.getHeight()-roborText.getHeight()+scale(5));
    robortoText.render(scale(120-robortoTexture.getWidth()/2)+robortoTexture.getWidth()+scale(5),scale(290)+robortoTexture.getHeight()-roborText.getHeight()+scale(5));
    robroText.render(scale(50-robroTexture.getWidth()/2)+robroTexture.getWidth()+scale(5),scale(330)+robroTexture.getHeight()-roborText.getHeight()+scale(5));
    romoText.render(scale(120-romoTexture.getWidth()/2)+romoTexture.getWidth()+scale(5),scale(400)+romoTexture.getHeight()-roborText.getHeight()+scale(5));
    rooText.render(scale(50-rooTexture.getWidth()/2)+rooTexture.getWidth()+scale(5),scale(410)+rooTexture.getHeight()-roborText.getHeight()+scale(5));

    upgradeHealth.w = scale(player->getHealthPercentage()*102);

    upgradeShield.w = scale(player->getShieldPercentage()*102);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer,&upgradeHealthBack);
    SDL_RenderFillRect(renderer,&upgradeShieldBack);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&upgradeHealth);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer,&upgradeShield);

}

void renderMenu(State& state) {
  if (UI_Menu *currentMenu = getCurrentMenu(state); currentMenu != nullptr) {
      currentMenu->render();
  }
    if(state.menu == weaponUpgrade || state.menu == abilityUpgrade || state.menu == playerUpgrade) {
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

    shieldBackRect.x = scale(35);
    shieldBackRect.y = WINDOW_HEIGHT-scale(80);
    shieldBackRect.w = scale(15);
    shieldBackRect.h = scale(75);

    shieldRect.x = scale(35);
    shieldRect.y = WINDOW_HEIGHT-scale(80);
    shieldRect.w = scale(15);
    shieldRect.h = scale(75);

    healthBackRect.x = scale(10);
    healthBackRect.y = WINDOW_HEIGHT-scale(80);
    healthBackRect.w = scale(15);
    healthBackRect.h = scale(75);

    healthRect.x = scale(10);
    healthRect.y = WINDOW_HEIGHT-scale(80);
    healthRect.w = scale(15);
    healthRect.h = scale(75);

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

    healthRect.h = scale(player->getHealthPercentage()*75);
    healthRect.y = WINDOW_HEIGHT-scale(80-75)-healthRect.h;

    shieldRect.h = scale(player->getShieldPercentage()*75);
    shieldRect.y = WINDOW_HEIGHT-scale(80-75)-shieldRect.h;

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer,&healthBackRect);
    SDL_RenderFillRect(renderer,&shieldBackRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&healthRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer,&shieldRect);

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

