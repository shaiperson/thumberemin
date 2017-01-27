#ifndef TRACKINGINFO_H
#define TRACKINGINFO_H

#include "config.h"

class TrackingInfo {
public:
    TrackingInfo() { }
    virtual ~TrackingInfo() { };

    virtual void update(const Mat& frame) = 0;
    virtual Point current() const = 0;
    virtual Mat takeSample(Mat& frame) = 0;

    Size windowSize() const { return dynconf.trackingWindowSize; }

protected:
    // tracking state (current position for example)
};

#endif
