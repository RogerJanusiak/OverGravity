//
// Created by Roger Janusiak on 10/16/24.
//

#include "../includes/Sound.h"

Sound::Sound(std::string filePath, int loop) : loop(loop) {

    sound = Mix_LoadWAV( filePath.c_str() );
    if( sound == nullptr ) {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }

}
Sound::~Sound() {
    Mix_FreeChunk(sound);
    sound = nullptr;
}

void Sound::play() {
    Mix_PlayChannel( -1, sound, loop );
}
