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

    struct PhaseFunctor {
        PhaseFunctor(ContinuousSineWave& x) : c(x) { }
        float operator()(float PHI) { return c.g (c.currFrequency, PHI) - c.g (c.prevFrequency, c.phase); }
    private:
        ContinuousSineWave& c;
    };

    struct TolFunctor {
        bool operator()(float,float) { return false; }
    };

    size_t sampleRate;
    float currFrequency;
    float prevFrequency;
    float amplitude;
    float phase;

    unsigned long long int increment;
    bool freqChange;

    const size_t maxBisectionIterations;


    PhaseFunctor phaseFunctor;
    TolFunctor tolFunctor;
};

#endif
