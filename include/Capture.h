#ifndef CAPTURE_H
#define CAPTURE_H

#include "global.h"

/* Wrapper class for cv::VideoCapture that provides flipped frames */

class Capture {
public:
    Capture(int cameraId) : cvcapture(cameraId) { }
    Size frameSize() { return Size(cvcapture.get(CV_CAP_PROP_FRAME_WIDTH), cvcapture.get(CV_CAP_PROP_FRAME_HEIGHT)); }
    bool isOpened() { return cvcapture.isOpened(); }

    Capture& operator>>(Mat& image) {
        Mat temp;
        cvcapture >> temp;
        flip(temp, image, 1);
        return *this;
    }

private:
    VideoCapture cvcapture;
};

#endif
