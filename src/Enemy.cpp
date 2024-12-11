#include "../includes/Enemy.h"

Enemy::Enemy(Entity *_entity) {
	entity = _entity;
}

void Enemy::move(float dt,const std::list<Platform*> &platforms, State& state) {
	if(entity->justSpawned) {
		entity->justSpawned = false;
		if(entity->getRect().x <=scale(400)) {
			entity->setXVelocity(getXVelocity());
		} else {
			entity->setXVelocity(-getXVelocity());
		}
	}
	entity->move(dt,platforms);
	if(entity->getRect().y >= scale(state.levelHeight)+state.camY) {
		entity->despawn();
	}

	if(entity->getRect().x >= WINDOW_WIDTH) {
		entity->setPosition(scale(10),entity->getRect().y);
	} else if(entity->getRect().x <= 0) {
		entity->setPosition(WINDOW_WIDTH-scale(10), entity->getRect().y);
	}


}

int Enemy::findEdgeRight(int startX, int startY, State& state) {
	bool lookingRight = true;
	int numberTilesRight = 1;
	while(lookingRight) {
		int rightProbe = startX+numberTilesRight;
		if(rightProbe > 16) {
			numberTilesRight = -1;
			lookingRight = false;
		} else {
			if(state.levelMap[startY][rightProbe] != -1) {
				numberTilesRight++;
			} else {
				lookingRight = false;
			}
		}
	}
	return numberTilesRight;
}

int Enemy::findEdgeLeft(int startX, int startY, State& state) {
	bool lookingLeft = true;
	int numberTilesLeft = 1;
	while(lookingLeft) {
		int leftProbe = startX-numberTilesLeft;
		if(leftProbe < 0) {
			numberTilesLeft = -1;
			lookingLeft = false;
		} else {
			if(state.levelMap[startY][leftProbe] != -1) {
				numberTilesLeft++;
			} else {
				lookingLeft = false;
			}
		}
	}
	return numberTilesLeft;
}