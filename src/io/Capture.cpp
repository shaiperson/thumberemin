#include "../../include/io/Capture.h"

Capture::Capture(int cameraId, int width, int height) :
    cvcapture(cameraId) {
        cvcapture.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cvcapture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }

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

bool Capture::read(Mat& image) {
    return cvcapture.read(image);
}

void Capture::release() {
    cvcapture.release();
}
