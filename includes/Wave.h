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

	[[nodiscard]] std::list<Entity>& getBulletEntities() { return eBullets; }
	[[nodiscard]] std::list<Bullet>& getBullets() { return bullets; }

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

	std::list<Entity> eBullets;
	std::list<Bullet> bullets;

	// TODO: See if any lists can be vectors and vectors arrays

};

#endif
