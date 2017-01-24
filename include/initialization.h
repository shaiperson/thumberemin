#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "global.h"

VideoCapture initializeVideoCapture();

struct InitialScreen {
    vector<Rect> inactiveRegions;
    Rect activeRegion;
    Rect samplingRegion;

    InitialScreen(const Size& size);
    void updateFrame(Mat& frame);
};

#endif
