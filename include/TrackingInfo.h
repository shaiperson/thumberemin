#ifndef TRACKER_H
#define TRACKER_H

#include "global.h"

class TrackingInfo {
public:
    Tracker();
    ~Tracker();

    virtual update() = 0;
    virtual current() = 0;

protected:
    // tracking state (current position for example)
};

class ColorSampler : public TrackingInfo {
public:
    update(const Mat& frame) {} // empty as fuck
    current(); // returns a fixed position for sampling
};

class Tracker : public TrackingInfo {
public:
    update(const Mat& frame) {} // do actual tracking and update state accordingly
    current(); // returns position according to tracker state as updated by update()
}

#endif
