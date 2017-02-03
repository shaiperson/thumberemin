#include "../include/ContinuousSineWave.h"

ContinuousSineWave::ContinuousSineWave(float frequency, size_t sampleRate) :
    sampleRate(sampleRate),
    increment(0),
    currFrequency(frequency),
    prevFrequency(frequency),
    amplitude(1),
    phase(0),
    freqChange(false),
    phaseFunctor(*this),
    maxBisectionIterations(50) { }

float ContinuousSineWave::nextSample() {
    // if (freqChange) {
    //     freqChange = false;
    //     updatePhase();
    // }

    float result = g(currFrequency, phase); // call f with current frequency and phase
    increment++;
    return result;
}

vector<float> ContinuousSineWave::nextCycle() {
    size_t tableSize = cycleSize();
    vector<float> result(tableSize);
    for (size_t i = 0; i < tableSize; ++i) {
        result[i] = nextSample();
    }
    return result;
}

void ContinuousSineWave::updateFrequency(float freq) {
    prevFrequency = currFrequency;
    currFrequency = freq;
    /* freqChange = true; */ updatePhase();
}

void ContinuousSineWave::updatePhase() {
    phase = phase + (2*M_PI * increment * (prevFrequency - currFrequency)) / sampleRate;
}
