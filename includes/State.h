#ifndef STATE_H
#define STATE_H

struct State {
    //Game State
        bool quit = false;
        bool paused = false;

    //Menu Status
        bool mainMenu = true;
        bool levelSelect = false;
        bool upgradeScreen = false;
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
        bool c4Placed;

};

#endif //STATE_H
