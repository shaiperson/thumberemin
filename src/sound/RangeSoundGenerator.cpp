/*#include "../../include/sound/RangeSoundGenerator.h"

RangeSoundGenerator::RangeSoundGenerator()
    : wave (
        dynconf.pixel2Freq[dynconf.samplingRegion.y + dynconf.samplingRegion.height/2],
        StaticConfiguration::sampleRate
    )
{
    PaError err;

    err = Pa_Initialize();
    initializeOutputParameters();

    err = Pa_OpenStream (
        &stream,
        NULL, // no input
        &outputParameters,
        StaticConfiguration::sampleRate,
        paFramesPerBufferUnspecified, // number of frames based on host requirements latency settings
        0, // no flags
        callback, // use blocking mode
        &wave
    );

    err = Pa_StartStream(stream);
}

void RangeSoundGenerator::update(const TrackingInfo& tracker) {
    wave.updateFrequency(dynconf.pixel2Freq[tracker.current().y]);
}
int RangeSoundGenerator::callback (
        const void* input,
        void* output_,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* wave_ ) {

    ContinuousSineWave* wavegen = (ContinuousSineWave*)wave_;
    float* output = (float*)output_;

    for (size_t i = 0; i < frameCount; ++i) {
        output[i] = wavegen->nextSample();
    }

    return paContinue;
}

void RangeSoundGenerator::initializeOutputParameters() {
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice)
        cerr << "Error finding default output device" << endl;

    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.channelCount = StaticConfiguration::numberOfChannels;
}
*/
