#include "../../include/tracking/ColorSampler.h"

void ColorSampler::update(const Mat& frame) {
    // empty as fuck
}

Point ColorSampler::current() const {
    return window.tl() + Point(window.width/2, window.height/2);
}

Mat ColorSampler::takeSample(Mat& frame) {
    Mat samplingData = frame(dynconf.samplingRegion);
    Mat hist = IHT_createHistArgumentShort();
    IHT_calc3DByteDepthUniformHist(&samplingData, &hist);
    return hist;
}
