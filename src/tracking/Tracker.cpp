#include "../../include/tracking/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, StaticConfiguration::termCritIters, 0) // terminate after 10 iterations
    { }

void Tracker::update(const Mat& frame) {
    /* Playing region expanded with exact margins to allow desired movement of tracking marker
    Note this strongly relies on meanShift()'s behavior with respect to tracking window reaching edges of frame*/
    Point playingRegionExpansionVector = Point(0, StaticConfiguration::trackingWindowSize.height / 2);
    Rect roiRect (
        dynconf.playingRegion.tl() - playingRegionExpansionVector,
        dynconf.playingRegion.br() + playingRegionExpansionVector
    );

    Mat roi = frame(roiRect);
    Mat backProjection = IHT_createBackProjectArgumentShort(roiRect.size());

    IHT_calc3DByteDepthBackProject(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);

    Point windowTranslation = Point(-dynconf.inactiveRegions[0].width, -roiRect.y);
    window += windowTranslation; // shift to playingRegion-relative position
    IHT_meanShift(backProjection, window, StaticConfiguration::termCritIters);
    window -= windowTranslation; // shift back to frame-relative position
}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
