#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "../includes/GlobalConstants.h"
#include "../includes/MainMenu.h"
#include "../includes/State.h"

bool init(GlobalGameState& ggs) {
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO ) < 0 ) {
		SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_Log( "TTF could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	ggs.window = SDL_CreateWindow("Hyper_Gravity 0.4.1 (Unstable)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (ggs.window == nullptr) {
		SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	ggs.renderer = SDL_CreateRenderer(ggs.window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ggs.renderer == nullptr) {
		SDL_Log("Game renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
		return false;
	}

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		SDL_Log( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}

	ggs.controller = SDL_GameControllerOpen(SDL_NumJoysticks()-1);
	if(ggs.controller != nullptr) {
		SDL_GameControllerAddMappingsFromFile("resources/mapping.txt");
	}

	return true;
}

void close(GlobalGameState ggs) {
	SDL_DestroyRenderer(ggs.renderer);
	ggs.renderer = nullptr;

	SDL_GameControllerClose(ggs.controller);
	ggs.controller = nullptr;

	SDL_DestroyWindow(ggs.window);
	ggs.window = nullptr;

	Mix_CloseAudio();

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void loadGlobalResources(GlobalGameState& gss) {

	gss.buttonFont = TTF_OpenFont("resources/sans.ttf",scaleUI(18));

}

int main( int argc, char* args[] ) {
	GlobalGameState ggs{};

	if(!init(ggs)) {
		SDL_Log("Initialization failed!\n");
		return 8;
	}

	loadGlobalResources(ggs);

	while(!ggs.quit) {
		runMainMenu(ggs);
	}

	close(ggs);
}