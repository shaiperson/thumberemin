#ifndef TRACKER_H
#define TRACKER_H

#include "global.h"
#include "TrackingInfo.h"

class Tracker : public TrackingInfo {
public:
    Tracker(Mat& histogram);
    void update(const Mat& frame); // do actual tracking and update state accordingly
    Point current() const; // returns position according to tracker state as updated by update()

    Mat takeSample(Mat&); // conforming

private:
    Mat sample;
};

#endif
