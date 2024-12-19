#ifndef RUN_H
#define RUN_H
#include "Level.h"
#include "../Entities/Player.h"

class Run {
public:

	explicit Run(GlobalGameState& ggs, int level);

	[[nodiscard]] Player& getPlayer() { return player; }
	[[nodiscard]] Level& getLevel() { return currentLevel; }

	[[nodiscard]] int getWaveNumber() const { return waveNumber; }
	void nextWave() { waveNumber++; }

private:

	GlobalGameState& ggs;

	Entity playerEntity;
	Player player;

	int waveNumber = 1;
	// TODO: Can spawn in either spawn
	Level currentLevel;

};



#endif //RUN_H
