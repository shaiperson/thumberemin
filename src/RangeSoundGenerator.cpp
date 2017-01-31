#include "../include/RangeSoundGenerator.h"

RangeSoundGenerator::RangeSoundGenerator() {
    PaError err;
    initializePixel2Freq();

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
    data.currSamples = createTable(pixel2Freq[samplingRegionCenterRow]);

    err = Pa_StartStream(stream);
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

void RangeSoundGenerator::update(const TrackingInfo& tracker) {
    float currentFreq = pixel2Freq[tracker.current().y];
    data.prevSamples = data.currSamples;
    data.currSamples = createTable(currentFreq);
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

    size_t prevTableSize = data->prevSamples.size();
    size_t currTableSize = data->currSamples.size();
    size_t oldNextSampleIdx = data->nextSampleIdx;

    // Start by finishing previous frequency's samples
    size_t j;
    for (j = oldNextSampleIdx; j < prevTableSize && j-oldNextSampleIdx < frameCount; ++j) {
        output[j-oldNextSampleIdx] = data->prevSamples[j];
    }

    // Continue with current frequency's samples
    size_t framesSoFar = j - oldNextSampleIdx;
    size_t newNextSampleIdx = 0;
    for (size_t i = framesSoFar; i < frameCount; ++i) {
        output[i] = data->currSamples[newNextSampleIdx++];
        newNextSampleIdx %= currTableSize;
    }

    data->nextSampleIdx = newNextSampleIdx;
    return paContinue;
}

void RangeSoundGenerator::initializePixel2Freq() {
    float freq0 = StaticConfiguration::noteRange[0];
    float freq1 = StaticConfiguration::noteRange[1];

    float freq0log = log2(freq0);
    float freq1log = log2(freq1);
    float d = freq1log-freq0log;

    size_t pixelRow = dynconf.playingRegion.y + dynconf.playingRegion.height - 1;
    for (size_t i = 0; i < StaticConfiguration::totalNotes; ++i) {
        for (size_t counter = 0; counter < dynconf.pixelsPerNote; ++counter) {
            pixel2Freq[pixelRow--] = exp2(freq0log + ((float)i / StaticConfiguration::totalNotes));
        }
    }
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
