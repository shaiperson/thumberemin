#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "global.h"

VideoCapture initializeVideoCapture();

struct InstructionsText {
    vector<string> lines;
    Point bottomLeft;
    int fontFace;
    double fontScale;
    int thickness;
    Size firstLineSize;
    Scalar color;
    int baseLine;

    InstructionsText(const vector<string>& lines, const Point& bottomLeft);
    void write(Mat&);
};

struct InitialScreen {
    vector<Rect> inactiveRegions;
    Rect activeRegion;
    Rect samplingRegion;

    InstructionsText samplingInstructions;

    InitialScreen(const Size& size);
    void updateFrame(Mat& frame);
};

#endif
