#ifndef ROBOR_H
#define ROBOR_H

#include "Entity.h"
#include "Platform.h"
#include "Enemy.h"

class Robor : public Enemy {

public:
	explicit Robor(Entity* _entity) : Enemy(_entity) {
		if(!entity->getTexture()->loadFromFile("robor.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	int getDifficulty() override { return 1; }

};

#endif
