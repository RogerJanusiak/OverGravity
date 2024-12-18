#ifndef RUN_H
#define RUN_H
#include "Level.h"
#include "Player.h"

class Run {
public:

	explicit Run(GlobalGameState& ggs);

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
	Level currentLevel = Level(ggs,4);

};



#endif //RUN_H
