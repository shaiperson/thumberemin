#ifndef CAPTURE_H
#define CAPTURE_H

#include "../global.h"

/* Wrapper class for cv::VideoCapture that provides flipped frames */

class Capture {
public:
    Capture(int cameraId);
    Size frameSize();
    bool isOpened();
    Capture& operator>>(Mat& image);
    bool read(Mat& image);
    void release();

private:
    VideoCapture cvcapture;
};

#endif
