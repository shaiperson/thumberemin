#include "../../include/io/Capture.h"

Capture::Capture(int cameraId) :
    cvcapture(cameraId) { }

Capture::Capture(const string& filepath) :
    cvcapture(filepath) { }

double Capture::get(int propId) {
    return cvcapture.get(propId);
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

void Capture::operator=(Capture& another) {
    cvcapture = another.cvcapture;
}
