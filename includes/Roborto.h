#ifndef ROBORTO_H
#define ROBORTO_H

#include <list>
#include "Enemy.h"
#include "Entity.h"
#include "Platform.h"
#include "State.h"

class Roborto : public Enemy {
public:
	explicit Roborto(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Roborto::getWidth(),Roborto::getHeight());
		entity->getTexture()->setup(Roborto::getWidth(),Roborto::getHeight(),entity->getRenderer());
		if(!entity->getTexture()->loadFromFile("roborto.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

    void move(float dt,const std::list<Platform*> &platforms, State& state) override;

    void pathFind(int x, int y, int& leftWeight, int& rightWeight, State state, bool firstCall);

	int getDifficulty() override { return difficulty; }

	static constexpr int difficulty = 2;

	[[nodiscard]] int getWidth() const override { return scale(28); }
	[[nodiscard]] int getHeight() const override { return scale(50); }

};

#endif //ROBORTO_H
