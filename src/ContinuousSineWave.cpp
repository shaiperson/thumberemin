#include "../include/ContinuousSineWave.h"
ContinuousSineWave::ContinuousSineWave(float f) :
    increment(0),
    currFrequency(f),
    prevFrequency(f),
    amplitude(1),
    phase(0),
    freqChange(false),
    phaseFunctor(*this) { }

float ContinuousSineWave::nextSample() {
    if (freqChange) {
        freqChange = false;
        updatePhase();
    }

    float result = f(currFrequency, phase); // call f with current frequency and phase
    increment++;
    return result;
}

void ContinuousSineWave::updateFrequency(float freq) {
    prevFrequency = currFrequency;
    currFrequency = freq;
    freqChange = true;
}

void ContinuousSineWave::updatePhase() {
    /* Perform bisection */
    float bracketMin = 0;
    float bracketMax = max(prevPeriod(), currPeriod());
    float mid = bracketMax/2;

    pair<float,float> bracket =
        boost::math::tools::bisect (
            phaseFunctor,
            bracketMin,
            bracketMax,
            tolFunctor,
            maxBisectionIterations
        );

    // TODO evaluate possibility of using additional Newton iterations

    phase = (bracket.second - bracket.first) / 2;
}
