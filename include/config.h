#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"

namespace StaticConfiguration {
    extern const int defaultCamera;
    extern const string gameWindowName;
    extern const double dimmingFactor;
    extern const Scalar trackingWindowColor;
    extern const int trackingWindowThickness;
    extern const int trackingMarkerRadius;
    extern const Scalar trackingMarkerColor;
    extern const int trackingMarkerThickness;
    extern const float noteRange[2];
    extern const size_t totalNotes;
}

struct DynamicConfiguration {
    Size frameSize;
    map<size_t, float> freqs;
    Rect playingRegion;
    vector<Rect> inactiveRegions;
    Rect samplingRegion;
    Size trackingWindowSize;

    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }
};

extern DynamicConfiguration dynconf;

#endif
