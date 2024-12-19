#ifndef ROBORTO_H
#define ROBORTO_H

#include <list>
#include "Enemy.h"
#include "Entity.h"
#include "../HEng/Platform.h"
#include "../HEng/State.h"

class Roborto : public Enemy {
public:
	explicit Roborto(Entity* _entity) : Enemy(_entity) {
		entity->setDimensions(Roborto::getWidth(),Roborto::getHeight());
		entity->getTexture()->setup(Roborto::getWidth(),Roborto::getHeight(),entity->getRenderer());
		entity->setSource(18,32);
		if(!entity->getTexture()->loadFromFile("roborto.png")) {
			SDL_Log("Could not load enemy texture!");
		}
	}

	ENEMY_TYPE getType() override { return roborto; }

    void move(GlobalGameState& ggs, const std::list<Platform> &platforms, Level& level) override;

    void pathFind(int x, int y, int& leftWeight, int& rightWeight, GlobalGameState& ggs, Level& level, bool firstCall);

	int getDifficulty() override { return difficulty; }

	static constexpr int difficulty = 2;
	static constexpr int health = 2;

	[[nodiscard]] float getXVelocity() const override { return scale(250); }

	[[nodiscard]] int getWidth() const override { return scale(28); }
	[[nodiscard]] int getHeight() const override { return scale(50); }

private:
	bool justHitPlatform = false;

};

#endif //ROBORTO_H
