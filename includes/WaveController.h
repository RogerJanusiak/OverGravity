#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Run.h"
#include "Wave.h"


class WaveController {

public:
	explicit WaveController(GlobalGameState& ggs, Run& run);

	void operate();
	void readInput();

private:

	void startWave();
	void operatePlayer();
	void startLevel() const;

	void renderPlayerUI();
	void updatePlayerUIText();
	void renderWaveText();
	void updateWaveText();

	void updateTimeToShoot(double width);

	GlobalGameState& ggs;

	Run& run;
	Player& player;
	std::unique_ptr<Wave> currentWave = nullptr;

	bool rightMovement = false;
	bool leftMovement = false;

	bool shootingReset = true;

	bool inWave = false;

	//UI Elements
	SDL_Rect timeToShootBack;
	SDL_Rect timeToShoot;

	SDL_Rect timeToAbilityBack;
	SDL_Rect timeToAbility;

	SDL_Rect healthBackRect;
	SDL_Rect healthRect;
	SDL_Rect shieldBackRect;
	SDL_Rect shieldRect;

	Texture healthText;
	Texture shieldText;

	Texture waveNumberText;
	Texture waveNumberTitle;
	Texture comboNumberText;
	Texture playerXPText;
	Texture fpsText;

};



#endif //CONTROLLER_H
