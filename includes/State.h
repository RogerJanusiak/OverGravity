#ifndef STATE_H
#define STATE_H

struct State {
    bool mainMenu;
    bool levelSelect;
    bool started;
    bool developerMode;
    int camY;
    int level;
    bool initialLoad;
};

#endif //STATE_H
