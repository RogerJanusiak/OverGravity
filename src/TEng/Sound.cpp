#include "../../includes/TEng/Sound.h"

Sound::Sound() {
    sound = nullptr;
    loop = 0;
    channel = -1;
}

void Sound::init(const std::string& filePath, int _loop, int _channel) {
    loop = _loop;
    channel = _channel;
    sound = Mix_LoadWAV( filePath.c_str() );
    if( sound == nullptr ) {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}


Sound::Sound(std::string filePath, int loop, int channel) : loop(loop), channel(channel) {

    init(filePath, loop, channel);

}
Sound::~Sound() {
    Mix_FreeChunk(sound);
    sound = nullptr;
}

void Sound::play() {
    Mix_PlayChannel( channel, sound, loop );
}
