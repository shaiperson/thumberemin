#ifndef TRACKINGINFO_H
#define TRACKINGINFO_H

#include "../config/config.h"

class TrackingInfo {
public:
    TrackingInfo() : window(dynconf.samplingRegion) { }
    virtual ~TrackingInfo() { };

    virtual void update(const Mat& frame) = 0;
    virtual Point current() const = 0;
    virtual Mat takeSample(Mat& frame) = 0;

    Size windowSize() const { return StaticConfiguration::trackingWindowSize; }

protected:
    Rect window;
};

#endif
