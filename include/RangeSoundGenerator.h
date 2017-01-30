#ifndef RANGESOUNDGENERATOR_H
#define RANGESOUNDGENERATOR_H

#include "SoundGenerator.h"

class RangeSoundGenerator : public SoundGenerator {
public:
    RangeSoundGenerator();
    ~RangeSoundGenerator() { PaError err = Pa_CloseStream( stream ); };
    void update(const TrackingInfo& tracker);
private:
    struct callbackData {
        size_t nextSampleIdx;
        vector<float> samples;
        callbackData() : nextSampleIdx(0) {}
    };

    unordered_map<size_t, float> pixel2Freq;
    PaStreamParameters outputParameters;
    PaStream* stream;
    callbackData data;

    vector<float> createTable(float freq);

    static int callback (
        const void* input,
        void* output_,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* data_
    );

    void initializePixel2Freq();
    void initializeOutputParameters();

};

#endif
