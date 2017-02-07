#include "../../include/tracking/Tracker.h"

Tracker::Tracker(SparseMat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, 10, 0) // terminate after 10 iterations
    { }

void Tracker::update(const Mat& frame) {
    int nimages = 1;
    const int channels[3] = {0,1,2};
    float singleBinRange[2] = {0, 256};
    const float* ranges[3] = {singleBinRange, singleBinRange, singleBinRange};

    Mat backProjection;

    /* Playing region expanded with exact margins to allow desired movement of tracking marker
    Note this strongly relies on meanShift()'s behavior with respect to tracking window reaching edges of frame*/
    Point playingRegionExpansionVector = Point(0, StaticConfiguration::trackingWindowSize.height / 2);
    Rect roiRect (
        dynconf.playingRegion.tl() - playingRegionExpansionVector,
        dynconf.playingRegion.br() + playingRegionExpansionVector
    );

    Mat roi = frame(roiRect);
    calcBackProject(&roi, nimages, channels, sample, backProjection, ranges);

    Point windowShiftVector = Point(-dynconf.inactiveRegions[0].width, -roiRect.y);
    window += windowShiftVector; // shift to playingRegion-relative position
    meanShift(backProjection, window, termCriteria);
    window -= windowShiftVector; // shift back to frame-relative position
}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

SparseMat Tracker::takeSample(Mat& frame) {
    return SparseMat();
}
