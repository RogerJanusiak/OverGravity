#ifndef SOUND_H
#define SOUND_H
#include <SDL_mixer.h>
#include <string>

class Sound {

public:

    Sound();
    Sound(std::string filePath, int loop, int channel);
    ~Sound();

    void init(const std::string& filePath, int _loop, int _channel);

    void play();

private:

    Mix_Chunk* sound;
    int loop;
    int channel;

};



#endif //SOUND_H
