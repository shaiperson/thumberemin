#include "../include/InstructionsText.h"

InstructionsText::InstructionsText(const vector<string>& lines)
: lines(lines) {
    fontFace = FONT_HERSHEY_SIMPLEX;
    fontScale = 0.4;
    thickness = 1;
    firstLineSize = getTextSize(lines[0], fontFace, fontScale, thickness, &baseLine);
    color = Scalar(255,255,255);
}

void InstructionsText::write(Mat& frame, const Point& bottomLeft) const {
    // cout << bottomLeft << endl;
    size_t lineIndex = 0;
    size_t totalLines = lines.size();
    for (const string& line : lines)
        putText (
            frame,
            line,
            Point(bottomLeft.x, bottomLeft.y - (totalLines-lineIndex++)*(firstLineSize.height+baseLine)),
            fontFace,
            fontScale,
            color
        );
}
