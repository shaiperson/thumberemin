#include "../include/Capture.h"

Capture::Capture(int cameraId) :
    cvcapture(cameraId) { }

Size Capture::frameSize() {
    return Size(cvcapture.get(CV_CAP_PROP_FRAME_WIDTH), cvcapture.get(CV_CAP_PROP_FRAME_HEIGHT));
}

bool Capture::isOpened() {
    return cvcapture.isOpened();
}

Capture& Capture::operator>>(Mat& image) {
    Mat temp;
    cvcapture >> temp;
    flip(temp, image, 1);
    return *this;
}
