#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Run.h"
#include "Wave.h"


class Controller {

public:
	explicit Controller(GlobalGameState& ggs, Run& run) : ggs(ggs), run(run) {
		startWave();
	}

	void runController() const;
	void readInput();

private:

	void startWave();

	GlobalGameState& ggs;

	Run& run;
	std::unique_ptr<Wave> currentWave = nullptr;

	bool inWave = false;

};



#endif //CONTROLLER_H
