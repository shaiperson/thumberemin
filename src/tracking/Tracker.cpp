#include "../../include/tracking/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, StaticConfiguration::termCritIters, 0)
    { }

void Tracker::update(const Mat& frame) {
    Mat roi = frame;
    Mat backProjection = IHT_createBackProjectArgumentShort(roi.size());

    RunningMode mode = dynconf.runningMode;

    if (mode == IDIOMATIC) {
        IHT_calc3DByteDepthBackProject_CV(roi, sample, backProjection);
        timer::trackingAccum += timer::t;
        IHT_meanShift_CV(backProjection, window, StaticConfiguration::termCritIters);
        timer::trackingAccum += timer::t;
    } else if (mode == POINTERS) {
        IHT_calc3DByteDepthBackProject(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);
        timer::trackingAccum += timer::t;
        IHT_meanShift(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);
        timer::trackingAccum += timer::t;
    } else if (mode == ASM) {
        IHT_calc3DByteDepthBackProject_ASM(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);
        timer::trackingAccum += timer::t;
        IHT_meanShift_ASM(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);
        timer::trackingAccum += timer::t;
    }

    timer::trackingRepetitions += 1;
}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
