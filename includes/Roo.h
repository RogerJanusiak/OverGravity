#ifndef ROO_H
#define ROO_H

#include "Enemy.h"

class Roo : public Enemy {

public:
	explicit Roo(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Roo::getWidth(),Roo::getHeight());
		entity->getTexture()->setup(Roo::getWidth(),Roo::getHeight(),entity->getRenderer());
		entity->setSource(23,32);
		if(!entity->getTexture()->loadFromFile("roo.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	int getDifficulty() override { return difficulty; }
	ENEMY_TYPE getType() override { return roo; }

	static constexpr int difficulty = 5;
	static constexpr int health = 5;

	[[nodiscard]] int getWidth() const override { return scale(36); }
	[[nodiscard]] int getHeight() const override { return scale(50); }

	[[nodiscard]] float getXVelocity() const override { return scale(200); }

};

#endif
