#ifndef TRACKER_H
#define TRACKER_H

#include "global.h"

class TrackingInfo {
public:
    TrackingInfo() { }
    virtual ~TrackingInfo() { };

    virtual void update(const Mat& frame) = 0;
    virtual Point current() const = 0;

    Size windowSize() const { return config::trackingWindowSize; }

protected:
    // tracking state (current position for example)
};

class ColorSampler : public TrackingInfo {
public:
    void update(const Mat& frame) { } // empty as fuck
    Point current() const { return config::samplingRegion.tl(); } // returns a fixed position for color sampling ()
};

class Tracker : public TrackingInfo {
public:
    /* TODO */ void update(const Mat& frame) {} // do actual tracking and update state accordingly
    /* TODO */ Point current() const { return Point(0,0); } // returns position according to tracker state as updated by update()
};

#endif
