#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "global.h"
#include "config.h"
#include "Tracker.h"
#include <portaudio.h>

class SoundGenerator {
    virtual void update(const Tracker& tracker) = 0;
};

class RangeSoundGenerator : public Sound {
    RangeSoundGenerator() {
        
    }
    void update(const Tracker& tracker);
};

class SilentSoundGenerator : public Sound {
    void update(const Tracker& tracker);
};

#endif
