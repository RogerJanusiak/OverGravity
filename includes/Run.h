#ifndef RUN_H
#define RUN_H
#include "Level.h"
#include "Player.h"

class Run {
public:

	explicit Run(GlobalGameState& ggs);

	[[nodiscard]] Player& getPlayer() { return player; }
	[[nodiscard]] Level& getLevel() { return currentLevel; }

private:

	GlobalGameState& ggs;

	Entity playerEntity;
	Player player;

	Level currentLevel = Level(ggs,2);

};



#endif //RUN_H
