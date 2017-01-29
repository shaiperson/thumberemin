#ifndef SILENTSOUNDGENERATOR_H
#define SILENTSOUNDGENERATOR_H

#include "SoundGenerator.h"

class SilentSoundGenerator : public SoundGenerator {
public:
    void update(const Tracker& tracker);
};

#endif
