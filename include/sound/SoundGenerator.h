#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include "../global.h"
#include "../config/config.h"
#include "../tracking/Tracker.h"

class SoundGenerator {
public:
    virtual ~SoundGenerator() { };
    virtual void update(const TrackingInfo& tracker) = 0;
};

#endif
