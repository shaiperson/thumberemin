#ifndef COLORSAMPLER_H
#define COLORSAMPLER_H

#include "../global.h"
#include "TrackingInfo.h"

#include "../in-house-tracking/iht.h"

class ColorSampler : public TrackingInfo {
public:
    void update(const Mat& frame); // empty
    Point current() const; // returns a fixed position for color sampling
    Mat takeSample(Mat& frame); // returns histogram

private:
    using TrackingInfo::window;
};

#endif
