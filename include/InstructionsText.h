#ifndef INSTRUCTIONSTEXT_H
#define INSTRUCTIONSTEXT_H

#include "global.h"

struct InstructionsText {
    vector<string> lines;
    // Point bottomLeft;
    int fontFace;
    double fontScale;
    int thickness;
    Size firstLineSize;
    Scalar color;
    int baseLine;

    // InstructionsText(const vector<string>& lines, const Point& bottomLeft);
    InstructionsText(const vector<string>& lines); // constructor que no toma posición; write() pasa a tomar posición
    void write(Mat&, const Point& bottomLeft) const;
};

#endif
