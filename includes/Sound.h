//
// Created by Roger Janusiak on 10/16/24.
//

#ifndef SOUND_H
#define SOUND_H
#include <SDL_mixer.h>
#include <string>


class Sound {

public:

    Sound(std::string filePath, int loop);
    ~Sound();

    void play();

private:

    Mix_Chunk* sound;
    int loop;

};



#endif //SOUND_H
