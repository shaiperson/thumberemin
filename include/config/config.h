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

    uchar midiLow;
    int totalNotes;
    unordered_map<size_t, uchar> pixel2MidiNote;

    DynamicConfiguration(const Size& frameSize, uchar midiLow, int totalNotes);
    DynamicConfiguration() { }

private:
    void initializePixel2MidiNote();
};

extern DynamicConfiguration dynconf;

#endif
