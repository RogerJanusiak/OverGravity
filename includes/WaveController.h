#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Run.h"
#include "Wave.h"


class WaveController {

public:
	explicit WaveController(GlobalGameState& ggs, Run& run) : ggs(ggs), run(run), player(run.getPlayer()) {
		startWave();
	}

	void operate() const;
	void readInput();

private:

	void startWave();

	GlobalGameState& ggs;

	Run& run;
	Player& player;
	std::unique_ptr<Wave> currentWave = nullptr;

	bool rightMovement = false;
	bool leftMovement = false;

	bool inWave = false;

};



#endif //CONTROLLER_H
