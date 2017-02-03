#include "../include/ContinuousSineWave.h"

ContinuousSineWave::ContinuousSineWave(float frequency, size_t sampleRate) :
    sampleRate(sampleRate),
    increment(0),
    currFrequency(frequency),
    prevFrequency(frequency),
    amplitude(1),
    phase(0) { }

float ContinuousSineWave::nextSample() {
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
    updatePhase();
}

void ContinuousSineWave::updatePhase() {
    float prevPhase = phase;
    phase = phase + (2*M_PI * increment * (prevFrequency - currFrequency)) / sampleRate;

    /* Check derivative sign at cutting point */
    float prevDerivative = amplitude * cos ( prevPhase + (2*M_PI * prevFrequency * increment) / sampleRate );
    float currDerivative = amplitude * cos ( phase + (2*M_PI * currFrequency * increment) / sampleRate );

    if (prevDerivative * currDerivative < 0)
        phase += period(currFrequency) / 2;
}
