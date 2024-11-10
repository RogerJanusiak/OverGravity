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

	void render() const { entity->render(true); }
	[[nodiscard]] Entity* getEntity() const { return entity; }
	void knifeColliding() { knifeCollision = true; }
	void knifeNotColliding() { knifeCollision = false; }
	[[nodiscard]] bool didAlreadyCollide() const { return knifeCollision; }

	static int findEdgeRight(int startX, int startY,  State& state);
	static int findEdgeLeft(int startX, int startY, State& state);

	[[nodiscard]] virtual float getXVelocity() const { return scale(250); }

	[[nodiscard]] virtual int getWidth() const { return scale(24); }
	[[nodiscard]] virtual int getHeight() const { return scale(50); }

protected:

	bool knifeCollision = false;

	Entity* entity;
};


#endif //ENEMY_H
