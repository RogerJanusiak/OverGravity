#ifndef RUN_H
#define RUN_H
#include "Player.h"

class Run {
public:

	explicit Run(GlobalGameState& ggs);

private:

	GlobalGameState& ggs;

	Entity m_playerEntity;
	Player m_player;

};



#endif //RUN_H
