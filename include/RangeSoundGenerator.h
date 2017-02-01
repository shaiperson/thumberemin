#ifndef RANGESOUNDGENERATOR_H
#define RANGESOUNDGENERATOR_H

#include "SoundGenerator.h"
#include "ContinuousSineWave.h"

class RangeSoundGenerator : public SoundGenerator {

public:
    RangeSoundGenerator();
    ~RangeSoundGenerator() { PaError err = Pa_CloseStream( stream ); };
    void update(const TrackingInfo& tracker);

private:

    static int callback (
        const void* input,
        void* output_,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* wave_
    );

    void initializeOutputParameters();

    PaStreamParameters outputParameters;
    PaStream* stream;
    ContinuousSineWave wave;

};

#endif
