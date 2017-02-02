#include "../include/ContinuousSineWave.h"

ContinuousSineWave::ContinuousSineWave(float frequency) :
    increment(0),
    currFrequency(frequency),
    prevFrequency(frequency),
    amplitude(1),
    phase(0),
    freqChange(false),
    phaseFunctor(*this),
    maxBisectionIterations(50) { }

float ContinuousSineWave::nextSample() {
    if (freqChange) {
        freqChange = false;
        updatePhase();
    }

    float result = f(currFrequency, phase); // call f with current frequency and phase
    increment++;
    return result;
}

vector<float> ContinuousSineWave::nextCycle() {
    size_t tableSize = StaticConfiguration::sampleRate / currFrequency;
    vector<float> result(tableSize);
    for (size_t i = 0; i < tableSize; ++i) {
        result[i] = nextSample();
    }
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

    boost::uintmax_t maxiters = maxBisectionIterations;

    pair<float,float> bracket =
        boost::math::tools::bisect (
            phaseFunctor,
            bracketMin,
            bracketMax,
            tolFunctor,
            maxiters
        );

    // TODO evaluate possibility of using additional Newton iterations

    phase = (bracket.second - bracket.first) / 2;
}
