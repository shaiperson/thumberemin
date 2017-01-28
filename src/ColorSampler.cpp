#include "../include/ColorSampler.h"

void ColorSampler::update(const Mat& frame) {
    // empty as fuck
}

Point ColorSampler::current() const {
    return dynconf.samplingRegion.tl();
}

Mat ColorSampler::takeSample(Mat& frame) {
    // Mat resultB;
    // Mat resultG;
    // Mat resultR;
    //
    // vector<Mat> bgrPlanes;
    // split(frame, bgrPlanes);
    // int histSize = 256;
    // float range[] = {0, 256}; // lower boundary inclusive, upper boundary exclusive
    // const float* histRange = {range};
    //
    // Mat samplingRegionMask = zeros(dynconf.frameSize, frame.type());
    // Mat maskSubregion = samplingRegionMask(dynconf.samplingRegion);
    // maskSubregion.setTo(1);
    //
    // calcHist( &bgrPlanes[0], 1, 0, samplingRegionMask, resultB, 1, &histSize, &histRange);
    // calcHist( &bgrPlanes[1], 1, 0, samplingRegionMask, resultG, 1, &histSize, &histRange);
    // calcHist( &bgrPlanes[2], 1, 0, samplingRegionMask, resultR, 1, &histSize, &histRange);

    /* INTENTO DE HISTOGRAMA 3D */
    Mat samplingRegionMask = Mat::zeros(dynconf.frameSize, CV_8UC1);
    Mat maskRoi = samplingRegionMask(dynconf.samplingRegion);
    maskRoi.setTo(1); // sets ROI values to 1 in samplingRegionMask

    int nimages = 1; // only one 3-channel image
    const int channels[3] = {0,1,2};
    int dims = 3; // 3 dims: B, G and R.
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    float singleBinRange[2] = {0, 256};
    const float* ranges[3] = {singleBinRange, singleBinRange, singleBinRange};

    Mat hist;
    calcHist(&frame, nimages, channels, samplingRegionMask, hist, dims, histSize, ranges);

    return hist;
}
