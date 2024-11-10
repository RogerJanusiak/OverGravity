#ifndef ROBRO_H
#define ROBRO_H
#include "Enemy.h"

class Robro : public Enemy {

public:

	explicit Robro(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Robro::getWidth(),Robro::getHeight());
		entity->getTexture()->setup(Robro::getWidth(),Robro::getHeight(),entity->getRenderer());
		if(!entity->getTexture()->loadFromFile("robro.png")) {
			SDL_Log("Could not load robro texture!");
		}
	}

	int getDifficulty() override { return difficulty; }

	static constexpr int difficulty = 3;

	[[nodiscard]] float getXVelocity() const override { return scale(300); }

	[[nodiscard]] int getWidth() const override { return scale(19); }
	[[nodiscard]] int getHeight() const override { return scale(50); }

};

#endif
