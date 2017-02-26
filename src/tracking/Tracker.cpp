#include "../../include/tracking/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, StaticConfiguration::termCritIters, 0)
    { }

void Tracker::update(const Mat& frame) {
    // Thereminless usa frame entero como ROI
    Mat roi = frame;

    // PRUEBO CON EL FRAME ENTERO a ver si SIMD se la banca
    Mat backProjection = IHT_createBackProjectArgumentShort(roi.size());

    IHT_calc3DByteDepthBackProject(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);
    // IHT_calc3DByteDepthBackProject_ASM(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);

    // IHT_meanShift_CV(backProjection, window, StaticConfiguration::termCritIters);
    IHT_meanShift(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);
    // IHT_meanShift_ASM(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);

}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
