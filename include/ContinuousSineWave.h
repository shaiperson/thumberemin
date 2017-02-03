#ifndef CONTINUOUSSINEWAVE_H
#define CONTINUOUSSINEWAVE_H

#include "global.h"
#include "config.h"

class ContinuousSineWave {
public:
    ContinuousSineWave(float frequency, size_t sampleRate);

    float nextSample();
    vector<float> nextCycle();

    size_t cycleSize() {
        return sampleRate / currFrequency;
    }

    void updateFrequency(float frequency);

private:
    void updatePhase();

    float g (float fq, float phi) {
        return amplitude * sin ( phi + (2*M_PI*fq*increment) / sampleRate );
    }

    float period(float frequency) {
        return 1/frequency;
    }

    size_t sampleRate;
    float currFrequency;
    float prevFrequency;
    float amplitude;
    float phase;

    unsigned long long int increment;
};

#endif
