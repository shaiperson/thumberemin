#ifndef COLORSAMPLER_H
#define COLORSAMPLER_H

#include "../global.h"
#include "TrackingInfo.h"

class ColorSampler : public TrackingInfo {
public:
    void update(const Mat& frame); // empty as fuck
    Point current() const; // returns a fixed position for color sampling ()
    SparseMat takeSample(Mat& frame); // returns histogram

private:
    using TrackingInfo::window;
};

#endif
