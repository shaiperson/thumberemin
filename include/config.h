#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"

namespace StaticConfiguation {
    extern const int defaultCamera;
    extern const string gameWindowName;
    extern const double dimmingFactor;
    extern const Scalar trackingWindowColor;
    extern const int trackingWindowThickness;
}

struct DynamicConfiguration {
    Size frameSize;
    Rect playingRegion;
    vector<Rect> inactiveRegions;
    Rect samplingRegion;
    Size trackingWindowSize;

    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }
};

extern DynamicConfiguration dynconf;

#endif
