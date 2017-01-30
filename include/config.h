#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"

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
    extern const Scalar keyboardContourColor;
    extern const int keyboardContourThickness;
    extern const float noteRange[2];
    extern const size_t totalNotes;
    extern const size_t sampleRate;
    extern const size_t numberOfChannels;
}

class DynamicConfiguration {
public:
    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }

    Size frameSize;
    size_t pixelsPerNote;
    map<size_t, float> freqs;
    Rect playingRegion;
    vector<Rect> inactiveRegions;
    Rect samplingRegion;
    int trackingMarkerRadius;
    unordered_map<size_t, float> pixel2Freq;

    vector<Rect> whiteKeysRects;
    vector<Rect> blackKeysRects;

private:
    void initializePixel2Freq();
    void initializeKeyboardRectangles();
};

extern DynamicConfiguration dynconf;

#endif
