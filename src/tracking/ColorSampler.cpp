#include "../../include/tracking/ColorSampler.h"

void ColorSampler::update(const Mat& frame) {
    // ColorSampler does not move tracking window
}

Point ColorSampler::current() const {
    return window.tl() + Point(window.width/2, window.height/2);
}

Mat ColorSampler::takeSample(Mat& frame) {
    Mat samplingData = frame(dynconf.samplingRegion);
    Mat hist = IHT_createHistArgumentShort();

    IHT_calc3DByteDepthUniformHist(samplingData.data, hist.data, samplingData.rows, samplingData.cols, samplingData.step);
    // IHT_calc3DByteDepthUniformHist_ASM(samplingData.data, hist.data, samplingData.rows, samplingData.cols, samplingData.step);

    return hist;
}
