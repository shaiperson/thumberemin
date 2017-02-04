#include "../include/ContinuousSineWave.h"

ContinuousSineWave::ContinuousSineWave(float frequency, size_t sampleRate) :
    sampleRate(sampleRate),
    increment(0),
    lastIncrement(0),
    currFrequency(frequency),
    prevFrequency(frequency),
    amplitude(1),
    phase(0) { }

float ContinuousSineWave::nextSample() {
    float result = sin ( k()*currFrequency + phase );
    increment++;
    return result;
}

size_t ContinuousSineWave::cycleSize() {
    return sampleRate / currFrequency;
}

void ContinuousSineWave::updateFrequency(float freq) {
    if (freq != currFrequency) { // comparison by != is OK since frequencies are fixed at runtime
        cerr << currFrequency << " " << freq << " " << increment-lastIncrement << endl;

        lastIncrement = increment;

        prevFrequency = currFrequency;
        currFrequency = freq;

        updatePhase();
    }
}

float ContinuousSineWave::k() {
    return (2*M_PI * increment) / sampleRate;
}

void ContinuousSineWave::updatePhase() {
    phase = k()*(prevFrequency - currFrequency) + phase;
}
