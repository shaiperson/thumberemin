#ifndef RANGESOUNDGENERATOR_H
#define RANGESOUNDGENERATOR_H

#include "SoundGenerator.h"

class RangeSoundGenerator : public SoundGenerator {
public:
    void update(const TrackingInfo& tracker);
};

#endif
