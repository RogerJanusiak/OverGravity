#ifndef ROMO_H
#define ROMO_H
#include "Enemy.h"

class Romo : public Enemy {

public:
	explicit Romo(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Romo::getWidth(),Romo::getHeight());
		entity->getTexture()->setup(Romo::getWidth(),Romo::getHeight(),entity->getRenderer());
		entity->setSource(20,13);
		if(!entity->getTexture()->loadFromFile("romo.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	int getDifficulty() override { return difficulty; }

	ENEMY_TYPE getType() override { return romo; }

	static constexpr int difficulty = 1;
	static constexpr int health = 1;

	[[nodiscard]] int getWidth() const override { return scale(31); }
	[[nodiscard]] int getHeight() const override { return scale(20); }

	[[nodiscard]] float getXVelocity() const override { return scale(300); }

};

#endif
