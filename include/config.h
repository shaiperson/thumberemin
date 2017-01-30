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
    extern const float noteRange[2];
    extern const size_t totalNotes;
}

struct DynamicConfiguration {
    Size frameSize;
    size_t pixelsPerNote;
    map<size_t, float> freqs;
    Rect playingRegion;
    vector<Rect> inactiveRegions;
    Rect samplingRegion;
    int trackingMarkerRadius;

    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }
};

extern DynamicConfiguration dynconf;

#endif
