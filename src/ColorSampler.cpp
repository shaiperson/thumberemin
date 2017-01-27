#include "../include/ColorSampler.h"

void ColorSampler::update(const Mat& frame) {
    // empty as fuck
}

Point ColorSampler::current() const {
    return dynconf.samplingRegion.tl();
}

Mat ColorSampler::takeSample(Mat& frame) {
    return Mat();
}
