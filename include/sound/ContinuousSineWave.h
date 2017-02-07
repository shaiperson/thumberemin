#ifndef CONTINUOUSSINEWAVE_H
#define CONTINUOUSSINEWAVE_H

#include "../global.h"
#include "../config/config.h"

class ContinuousSineWave {
public:
    ContinuousSineWave(float frequency, size_t sampleRate);
    float nextSample();
    size_t cycleSize();
    void updateFrequency(float frequency);

private:
    float k();
    void updatePhase();

    size_t sampleRate;
    float currFrequency;
    float prevFrequency;
    float amplitude;
    float phase;
    unsigned long long int increment;
};

#endif
