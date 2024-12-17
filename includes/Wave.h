#ifndef WAVE_H
#define WAVE_H

#include "Enemy.h"
#include "Level.h"
#include "Player.h"

class Wave {

public:

	explicit Wave(GlobalGameState& ggs, Player& timpy, Level& level, int waveNumber);

	bool runWave();
	void render() const;

private:

	void createEnemies();

	GlobalGameState& ggs;
	Player& timpy;
	Level& level;
	int waveNumber = 0;

	int enemiesAlive = 0;

	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::list<Entity*> allCharacterEntities;

};

#endif
