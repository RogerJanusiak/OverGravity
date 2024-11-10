#ifndef ROMO_H
#define ROMO_H
#include "Enemy.h"

class Romo : public Enemy {

public:
	explicit Romo(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Romo::getWidth(),Romo::getHeight());
		entity->getTexture()->setup(Romo::getWidth(),Romo::getHeight(),entity->getRenderer());
		if(!entity->getTexture()->loadFromFile("romo.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	int getDifficulty() override { return difficulty; }

	static constexpr int difficulty = 2;
	static constexpr int health = 1;

	[[nodiscard]] int getWidth() const override { return scale(31); }
	[[nodiscard]] int getHeight() const override { return scale(20); }

	[[nodiscard]] float getXVelocity() const override { return scale(400); }

};

#endif
