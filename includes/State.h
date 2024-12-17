#ifndef STATE_H
#define STATE_H

#include <SDL_gamecontroller.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_ttf.h>
#include <vector>

#include "../includes/GlobalConstants.h"

enum Menu {
    head,
    level,
    settings,
    pause,
    weaponUpgrade,
    abilityUpgrade,
    playerUpgrade,
    notInMenu,
};

struct GlobalGameState {
    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_GameController *controller{};

    bool quit = false;

    bool inMainMenu = true;
    bool inRun = false;

    int level = 0;
    bool controllerStickReset = false;

    float dt = 0;

    int playerTileX = 0;
    int playerTileY = 0;
    int playerX = 0;

    //Developer Options
    bool developerMode = false;
    bool pauseEnemies = false;

    //Global Resources
    SDL_Color white = {255,255,255};

    TTF_Font *buttonFont{};
};


struct State {
    //Game State
    bool quit = false;


    bool teleportSelection = false;
    int teleportCursorX = 0;
    int teleportCursorY = 0;
    int tcVx = 0;
    int tcVy = 0;
    bool tcMovingRight = false;
    bool tcMovingLeft = false;
    bool tcMovingUp = false;
    bool tcMovingDown = false;
    float startSelection = 0;

    bool c4Exploded = false;

    //Menu Status
    Menu menu = head;
    bool controllerStickReset = true;

    //Load Status
    bool started = false ;
    bool developerMode = false;
    bool initialLoad = false;

    //Level Status
    int camY = 0;
    int camV = 0;
    int camA = scale(2000);
    int level = 0;
    int levelHeight = 0;
    std::vector<std::vector<int>> levelMap;

    //Wave Status
    int enemiesAlive = 0;

    //Control State
    bool controller = false;
    bool resetShooting = true;

    //Player Status
    int playerTileX = 0;
    int playerTileY = 0;
    int playerX = 0;

    //Weapon Status

    bool c4Placed = false;
    int numberOfWeapons = 5;
    int weapon1 = 0;
    int weapon2 = -1;

    int upgradeIncreaseFactor = 0;

    int numberOfAbilities = 3;

    int weaponLevels[5] = {1,0,0,0,0};
    int abilityLevels[4] = {0,0,0,0};
    int playerLevels[4] = {0,0,0,0};

    int abilitiesKills = 0;

    int numberOfPlayerUpgrades = 4;

    double weaponProperties[5][5][6] = {
        {{10,4,3,1,1,1},{20,5,3,1,1,1},{30,6,2,2,1,1},{40,7,2,2,2,1},{50,8,1,2,2,1}}, //Revolver
        {{10,2,3,1,2,1},{20,2,3,2,2,2},{30,3,2,2,3,2},{40,3,2,2,4,3},{50,4,2,2,5,3}}, //Rifle
        {{10,1,3,1,1,3},{20,1,3,1,1,4},{30,2,2,1,1,5},{40,2,2,1,1,6},{50,3,1,1,1,7}}, //Shotgun
        {{10,1,0,0,0,0},{20,1,0,0,0,0},{30,2,0,0,0,0},{40,2,0,0,0,0},{50,3,0,0,0,0}}, //Knife
        {{10,0.5,2,3,0,0},{20,0.5,3,3,0,0},{30,0.4,4,2,0,0},{40,0.4,5,2,0,0},{50,0.3,6,2,0,0}}}; //Laser Pistol

    int abilityProperties[4][5][3] = {
        {{10,10,0},{20,8,0},{30,6,0},{40,4,0},{50,2,0}},
        {{10,15,2},{20,12,2},{30,10,3},{40,8,3},{50,5,3}},
        {{10,15,1},{20,15,2},{30,12,3},{40,12,4},{50,10,5}},
        {{10,5,1},{20,4,1},{20,4,2},{40,3,2},{50,3,3}}
    };

    double playerProperties[4][5][2] = {
        {{10,10},{20,20},{30,30},{40,40},{50,50}}, //Armor
        {{10,1.25},{20,1.5},{30,1.75},{40,2},{50,2.5}}, //Shield Charge
        {{10,10},{20,20},{30,30},{40,40},{50,50}}, //Speed
        {{10,1},{20,2},{30,3},{40,4},{50,5}} // Dodge
    };

    //Enemy Weights
    static constexpr int numberOfEnemies = 5;
    int setEnemies[5] = {};
    int standardWeight[numberOfEnemies] = {40,75,89,90,100}; // Robor : Roborto : Robro : Romo : Roo
    int ductWeight[numberOfEnemies] = {1,1,1,99,100};
    int airportWeight[numberOfEnemies] = {1,97,98,99,100};
    int (*currentWeight)[numberOfEnemies] = &standardWeight;
};

#endif //STATE_H
