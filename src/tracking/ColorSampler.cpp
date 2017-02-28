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

    RunningMode mode = dynconf.runningMode;

    if (mode == IDIOMATIC) {
        IHT_calc3DByteDepthUniformHist_CV(samplingData, hist);
    } else if (mode == POINTERS) {
        IHT_calc3DByteDepthUniformHist(samplingData.data, hist.data, samplingData.rows, samplingData.cols, samplingData.step);
    } else if (mode == ASM) {
        IHT_calc3DByteDepthUniformHist_ASM(samplingData.data, hist.data, samplingData.rows, samplingData.cols, samplingData.step);
    }

    timer::sampling = timer::t;

    return hist;
}
