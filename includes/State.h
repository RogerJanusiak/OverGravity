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
        bool fullHealth = false;
        bool fullShield = false;
        int playerXP = 0;

    //Weapon Status

        bool c4Placed = false;
        int numberOfWeapons = 5;
        int currentRevolverLevel = 1;
        int currentRifleLevel = 0;
        int currentShotgunLevel = 0;
        int currentKnifeLevel = 0;
        int currentLaserPistolLevel = 0;
        int weapon1 = 0;
        int weapon2 = -1;

        float weaponLevel[5][5][6] = {
            {{10,4,3,1,1,1},{20,5,3,1,1,1},{30,6,2,2,1,1},{40,7,2,2,2,1},{50,8,1,2,2,1}}, //Revolver
            {{10,2,3,1,1,1},{20,2,3,2,2,2},{30,3,2,2,3,2},{40,3,2,2,4,3},{50,4,2,2,5,3}}, //Rifle
            {{10,1,3,1,1,3},{20,1,3,1,1,4},{30,2,2,1,1,5},{40,2,2,1,1,6},{50,3,1,1,1,7}}, //Shotgun
            {{10,1,0,0,0,0},{20,1,0,0,0,0},{30,2,0,0,0,0},{40,2,0,0,0,0},{50,3,0,0,0,0}}, //Knife
            {{10,0.5,2,3,0,0},{20,0.5,3,3,0,0},{30,0.4,4,2,0,0},{40,0.4,5,2,0,0},{50,0.3,6,2,0,0}}}; //Laser Pistol

};

#endif //STATE_H
