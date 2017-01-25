#ifndef INSTRUCTIONSTEXT_H
#define INSTRUCTIONSTEXT_H

#include "global.h"

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

#endif
