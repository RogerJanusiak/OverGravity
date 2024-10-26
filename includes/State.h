#ifndef STATE_H
#define STATE_H

#include <vector>

enum Menu {
    head,
    level,
    settings,
    pause,
    upgrade,
    notInMenu,
};

struct State {
    //Game State
        bool quit = false;

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

    //Player Status
        int playerTileX = 0;
        int playerTileY = 0;
        int playerX = 0;

    //Weapon Status

        //TODO: Reset on new game
        //TODO: Can't shoot right after upgrade screen
        bool c4Placed;
        int numberOfWeapons = 5;
        int currentRevolverLevel = 1;
        int currentRifleLevel = 0;
        int currentShotgunLevel = 0;
        int currentKnifeLevel = 0;
        int currentLaserPistolLevel = 0;
        int weapon1 = 0;
        int weapon2 = -1;

};

#endif //STATE_H
