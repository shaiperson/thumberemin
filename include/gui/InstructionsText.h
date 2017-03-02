#ifndef INSTRUCTIONSTEXT_H
#define INSTRUCTIONSTEXT_H

#include "../global.h"

struct InstructionsText {
    vector<string> lines;
    int fontFace;
    double fontScale;
    int thickness;
    Size firstLineSize;
    Scalar color;
    int baseLine;

    InstructionsText(const vector<string>& lines); // lines should be provided in desired order of appearance
    void write(Mat&, const Point& bottomLeft) const;
    int maxLineWidth();
};

#endif
