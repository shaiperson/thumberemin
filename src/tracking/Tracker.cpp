#include "../../include/tracking/Tracker.h"

Tracker::Tracker(Mat& histogram) :
    sample(histogram), // begin with samplingRegion as initial window
    termCriteria(TermCriteria::COUNT, StaticConfiguration::termCritIters, 0)
    { }

void Tracker::update(const Mat& frame) {
    int nimages = 1;
    const int channels[3] = {0,1,2};
    float singleBinRange[2] = {0, 256};
    const float* ranges[3] = {singleBinRange, singleBinRange, singleBinRange};
    Mat backProjection;

    // Thereminless usa frame entero como ROI
    Mat roi = frame;

    // PRUEBO CON EL FRAME ENTERO a ver si SIMD se la banca
    // backProjection = IHT_createBackProjectArgumentShort(roi.size());

    calcBackProject(&roi, nimages, channels, sample, backProjection, ranges);
    // IHT_calc3DByteDepthBackProject(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);
    // IHT_calc3DByteDepthBackProject_ASM(roi.data, sample.data, backProjection.data, roi.rows, roi.cols, roi.step);

    meanShift(backProjection, window, termCriteria);
    // IHT_meanShift(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);
    // IHT_meanShift_CV(backProjection, window, StaticConfiguration::termCritIters);
    // IHT_meanShift_ASM(backProjection.data, backProjection.rows, backProjection.cols, backProjection.step, &window, StaticConfiguration::termCritIters);

}

Point Tracker::current() const {
    return window.tl() + Point(window.width/2, window.height/2); // center point
}

Mat Tracker::takeSample(Mat& frame) {
    return Mat();
}
