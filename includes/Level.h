#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include <map>
#include <vector>
#include <__format/range_default_formatter.h>

#include "Entity.h"
#include "Platform.h"
#include "Spawn.h"
#include "State.h"

class Level {
public:

	Level(GlobalGameState& ggs, int level);

	void loadFromCSV(int level);
	void selectWeights(int level);
	void updateSpawns(const std::list<Entity*>& allCharacterEntities) const;
	void render() const;

	[[nodiscard]] int (* getSpawnWeights() const)[5] { return spawnWeights; }
	[[nodiscard]] std::vector<Spawn>* getEnemySpawns() { return &enemySpawns; }
	[[nodiscard]] std::list<Platform>& getPlatforms() { return platforms; }
	[[nodiscard]] std::vector<std::vector<int>>& getMap() {	return levelMap; }
	[[nodiscard]] std::vector<SDL_Rect>& getTeleports() { return teleports; }

private:

	GlobalGameState& ggs;

	int standardWeights[numberOfEnemies] = {40,75,89,90,100};
	int ductWeights[numberOfEnemies] = {1,1,1,99,100};
	int airportWeights[numberOfEnemies] = {1,97,98,99,100};

	int (*spawnWeights)[5] = &standardWeights;

	std::vector<Spawn> playerSpawns;
	std::vector<Spawn> enemySpawns;
	std::vector<Spawn*> allSpawns;

	//TODO: See if I can use vector, must wait to change until version is working
	std::list<Platform> platforms;

	std::vector<SDL_Rect> teleports;

	const int height = 9;

	std::vector<std::vector<int>> levelMap;

};

#endif //LEVEL_H
