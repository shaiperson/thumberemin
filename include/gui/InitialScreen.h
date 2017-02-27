#ifndef INITIALSCREEN_H
#define INITIALSCREEN_H

#include "Screen.h"

class InitialScreen : public Screen {
public:
    InitialScreen();

private:
    void processFrame(Mat&, const TrackingInfo&) const;

    InstructionsText samplingInstructions;
    void drawRectangleWithCenterAndSize(Mat& frame, const Point& center, const Size& size) const;
};

#endif
