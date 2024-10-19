#ifndef STATE_H
#define STATE_H
#include "Player.h"

struct State {
    //Menu Status
        bool mainMenu = true;
        bool levelSelect = false;

    //Load Status
        bool started = false ;
        bool developerMode = false;
        bool initialLoad = false;

    //Level Status
        int camY = 0;
        int level = 0;
        int levelHeight = 0;
        std::vector<std::vector<int>> levelMap;

    //Player Status
        int playerTileX = 0;
        int playerTileY = 0;
        int playerX = 0;
        Player* player = nullptr;

};

#endif //STATE_H
