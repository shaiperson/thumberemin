#include "../include/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram),
    window(dynconf.samplingRegion), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, 10, 0) // terminate after 10 iterations
    { }

void Tracker::update(const Mat& frame) {
    int nimages = 1;
    const int channels[3] = {0,1,2};
    float singleBinRange[2] = {0, 256};
    const float* ranges[3] = {singleBinRange, singleBinRange, singleBinRange};

    // Mat roi = frame(dynconf.playingRegion);
    // imshow("OMG POTAOTES", playingRegion);
    Mat backProjection;
    // calcBackProject(&roi, nimages, channels, sample, backProjection, ranges);
    calcBackProject(&frame, nimages, channels, sample, backProjection, ranges);

    meanShift(backProjection, window, termCriteria);
}

Point Tracker::current() const {
    return window.tl();
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
