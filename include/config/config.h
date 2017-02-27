#ifndef CONFIG_H
#define CONFIG_H

#include "../global.h"

namespace StaticConfiguration {
    extern const int defaultCamera;
    extern const string gameWindowName;
    extern const Size trackingWindowSize;
    extern const Scalar samplingWindowColor;
    extern const int samplingWindowThickness;
    extern const Scalar trackingMarkerColor;
    extern const int trackingMarkerThickness;
    extern const size_t termCritIters;
}

class DynamicConfiguration {
public:
    Size frameSize;
    Rect samplingRegion;
    int trackingMarkerRadius;

    DynamicConfiguration(const Size&);
    DynamicConfiguration() { }

private:
    void initializePixel2Freq();
};

extern DynamicConfiguration dynconf;

#endif
