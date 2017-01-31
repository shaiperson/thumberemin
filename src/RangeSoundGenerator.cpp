#include "../include/RangeSoundGenerator.h"

RangeSoundGenerator::RangeSoundGenerator() {
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
        &data
    );

    // fill data samples table with first note, corresponding to sampling region center position
    size_t samplingRegionCenterRow = dynconf.samplingRegion.y + dynconf.samplingRegion.height/2;
    size_t samplingRegionCenterFreq = dynconf.pixel2Freq[samplingRegionCenterRow];
    data.freq = samplingRegionCenterFreq;
    data.samples = createTable(samplingRegionCenterFreq);

    err = Pa_StartStream(stream);
}

void RangeSoundGenerator::update(const TrackingInfo& tracker) {
    float newFreq = dynconf.pixel2Freq[tracker.current().y];
    /* Update frequency information only if necessary.
    Comparing these floats by operator!= is OK since frequencies are fixed */
    if (data.freq != newFreq) {
        data.freq = newFreq;
        data.samples = createTable(newFreq);
    }
}

vector<float> RangeSoundGenerator::createTable(float freq) {
    float d = StaticConfiguration::sampleRate/freq;
    size_t tableSize = (size_t)d;
    vector<float> result(tableSize);
    for (size_t i = 0; i < tableSize; ++i) {
        result[i] = sin(2 * M_PI * (i/d));
    }

    return result;
}

int RangeSoundGenerator::callback (
                const void* input,
                void* output_,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void* data_ ) {

    callbackData* data = (callbackData*)data_;
    float* output = (float*)output_;

    size_t tableSize = data->samples.size();
    size_t nextSampleIdx = data->nextSampleIdx;

    for (size_t i = 0; i < frameCount; ++i) {
        output[i] = data->samples[nextSampleIdx++];
        nextSampleIdx %= tableSize;
    }

    data->nextSampleIdx = nextSampleIdx;
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
