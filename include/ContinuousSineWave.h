#ifndef CONTINUOUSSINEWAVE_H
#define CONTINUOUSSINEWAVE_H

#include "global.h"
#include "config.h"

class ContinuousSineWave {
public:
    ContinuousSineWave(float frequency);

    float nextSample();
    vector<float> nextCycle();

    void updateFrequency(float frequency);

private:
    void updatePhase();

    float f (float fq, float phi) {
        return amplitude * sin ( phi + (2*M_PI*fq*increment) / StaticConfiguration::sampleRate );
    }

    float prevPeriod() {
        return 1/prevFrequency;
    }

    float currPeriod() {
        return 1/currFrequency;
    }

    struct PhaseFunctor {
        PhaseFunctor(ContinuousSineWave& x) : c(x) { }
        float operator()(float PHI) { return c.f (c.currFrequency, PHI) - c.f (c.prevFrequency, c.phase); }
    private:
        ContinuousSineWave& c;
    };

    struct TolFunctor {
        bool operator()(float,float) { return false; }
    };

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
