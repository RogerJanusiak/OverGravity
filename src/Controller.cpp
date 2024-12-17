#include <memory>

#include "../includes/Controller.h"

void Controller::runController() const {
	if(currentWave != nullptr) {
		currentWave->render();
		currentWave->runWave();
	}
}

void Controller::startWave() {
	inWave = true;
	currentWave = std::make_unique<Wave>(ggs,run.getPlayer(), run.getLevel(), run.getWaveNumber());
}

void Controller::readInput() {
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
		if( e.type == SDL_QUIT ) {
			ggs.quit = true;
		}
	}
}

