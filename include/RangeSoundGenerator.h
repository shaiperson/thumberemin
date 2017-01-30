#ifndef RANGESOUNDGENERATOR_H
#define RANGESOUNDGENERATOR_H

#include "SoundGenerator.h"

class RangeSoundGenerator : public SoundGenerator {
public:
    RangeSoundGenerator();
    void update(const TrackingInfo& tracker);
private:
    unordered_map<size_t, float> pixel2Freq;
};

#endif
