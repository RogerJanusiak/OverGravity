#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "Level.h"
#include "State.h"

enum ENEMY_TYPE {
	robor,
	roborto,
	robro,
	romo,
	roo
};

class Enemy {

public:
	virtual ~Enemy() = default;
	explicit Enemy(Entity* _entity);

	virtual void move(GlobalGameState& ggs, const std::list<Platform> &platforms, Level& level);
	virtual int getDifficulty() { return 1; }
	virtual ENEMY_TYPE getType() { return robor; }

	void render() const { entity->render(entity->getHP()-1,0,true); }
	[[nodiscard]] Entity* getEntity() const { return entity; }
	void knifeColliding() { knifeCollision = true; }
	void knifeNotColliding() { knifeCollision = false; }
	[[nodiscard]] bool didAlreadyCollide() const { return knifeCollision; }

	static int findEdgeRight(int startX, int startY,  Level& level);
	static int findEdgeLeft(int startX, int startY, Level& level);

	[[nodiscard]] virtual float getXVelocity() const { return scale(250); }

	[[nodiscard]] virtual int getWidth() const { return scale(24); }
	[[nodiscard]] virtual int getHeight() const { return scale(50); }

protected:

	bool knifeCollision = false;

	Entity* entity;
};


#endif //ENEMY_H
