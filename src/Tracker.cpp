#include "../include/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, 10, 0) // terminate after 10 iterations
    { }

void Tracker::update(const Mat& frame) {
    int nimages = 1;
    const int channels[3] = {0,1,2};
    float singleBinRange[2] = {0, 256};
    const float* ranges[3] = {singleBinRange, singleBinRange, singleBinRange};

    Mat backProjection;

    Mat roi = frame(dynconf.playingRegion);
    calcBackProject(&roi, nimages, channels, sample, backProjection, ranges);

    Point windowShift = Point(-dynconf.inactiveRegions[0].width, -dynconf.playingRegion.y);
    window += windowShift; // shift to playingRegion-relative position
    meanShift(backProjection, window, termCriteria);
    window -= windowShift; // shift back to frame-relative position
}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
