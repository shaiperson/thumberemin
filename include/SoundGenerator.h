#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "global.h"
#include "config.h"
#include "Tracker.h"
#include <portaudio.h>

class SoundGenerator {
public:
    virtual ~SoundGenerator() { };
    virtual void update(const Tracker& tracker) = 0;
};

#endif
