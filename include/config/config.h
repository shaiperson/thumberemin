#ifndef CONFIG_H
#define CONFIG_H

#include "../global.h"

namespace StaticConfiguration {
    extern const int defaultCamera;
    extern const string gameWindowName;
    extern const double screenDivisionFactor;
    extern const double dimmingFactor;
    extern const Size trackingWindowSize;
    extern const Scalar samplingWindowColor;
    extern const int samplingWindowThickness;
    extern const Scalar trackingMarkerColor;
    extern const int trackingMarkerThickness;
    extern const uchar midiLow;
    extern const size_t totalNotes;
    extern const size_t sampleRate;
    extern const size_t numberOfChannels;
}

class DynamicConfiguration {
public:
    Size frameSize;
    size_t pixelsPerNote;
    map<size_t, float> freqs;
    Rect playingRegion;
    vector<Rect> inactiveRegions;
    Rect samplingRegion;
    int trackingMarkerRadius;
    unordered_map<size_t, uchar> pixel2MidiNote;

    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }

private:
    void initializePixel2MidiNote();
};

extern DynamicConfiguration dynconf;

#endif
