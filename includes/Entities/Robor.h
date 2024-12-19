#ifndef ROBOR_H
#define ROBOR_H

#include "Entity.h"
#include "Enemy.h"

class Robor : public Enemy {

public:
	explicit Robor(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Enemy::getWidth(),Enemy::getHeight());
		entity->getTexture()->setup(Enemy::getWidth(),Enemy::getHeight(),entity->getRenderer());
		entity->setSource(8,16);
		if(!entity->getTexture()->loadFromFile("robor.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	int getDifficulty() override { return difficulty; }

	static constexpr int difficulty = 1;
	static constexpr int health = 1;

};

#endif
