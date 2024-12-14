#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"
#include "Sound.h"
#include "State.h"
#include "Texture.h"

class MainMenu {
public:

	explicit MainMenu(GlobalGameState& ggs);
	void render() const;
	void readInput();
	void changeMenu(Menu menu);

private:

	void initMenus();

	GlobalGameState& ggs;

	Sound buttonSound;
	Texture logoTexture;

	UI_Menu mainMenu = UI_Menu(3);
	UI_Menu levelSelect = UI_Menu(4);

	UI_Menu* currentMenu = &mainMenu;

	bool levelSelected = false;

};

void setCurrentMainMenu(MainMenu* mainMenu);

#endif //MAINMENU_H
