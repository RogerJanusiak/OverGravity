#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include "State.h"


class Enemy {

public:
	virtual ~Enemy() = default;
	explicit Enemy(Entity* _entity);

	virtual void move(float dt,const std::list<Platform*> &platforms, State& state);
	virtual int getDifficulty() { return 1; }

	void render() const { entity->render(); }
	Entity* getEntity() const { return entity; }
	void knifeColliding() { knifeCollision = true; }
	void knifeNotColliding() { knifeCollision = false; }
	[[nodiscard]] bool didAlreadyCollide() const { return knifeCollision; }

	static int findEdgeRight(int startX, int startY,  State& state);
	static int findEdgeLeft(int startX, int startY, State& state);

protected:
	const int enemyWidth = scale(24);
	const int enemyHeight = scale(50);

	const int xVelocity = scale(250);

	bool knifeCollision = false;

	Entity* entity;
};


#endif //ENEMY_H
