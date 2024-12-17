#ifndef WAVE_H
#define WAVE_H
#include "Level.h"
#include "Run.h"


class Wave {

public:

	explicit Wave(Level& level) : level(level) {}

	void render() const;

private:

	Level& level;

};


#endif
