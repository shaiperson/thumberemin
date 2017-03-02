#include "../../include/gui/InstructionsText.h"

InstructionsText::InstructionsText(const vector<string>& lines)
: lines(lines) {
    fontFace = FONT_HERSHEY_SIMPLEX;
    fontScale = 0.4;
    thickness = 1;
    firstLineSize = getTextSize(lines[0], fontFace, fontScale, thickness, &baseLine);
    color = Scalar(255,255,255);
}

void InstructionsText::write(Mat& frame, const Point& bottomLeft) const {
    size_t lineIndex = 0;
    size_t totalLines = lines.size();
    for (const string& line : lines)
        putText (
            frame,
            line,
            Point(bottomLeft.x, bottomLeft.y - (totalLines-lineIndex++)*(firstLineSize.height)),
            fontFace,
            fontScale,
            color,
            1,
            CV_AA
        );
}

int InstructionsText::maxLineWidth() {
    vector<int> linesWidths(lines.size());

    transform (
        lines.cbegin(),
        lines.cend(),
        linesWidths.begin(),
        [this] (const string& s) { return getTextSize(s, fontFace, fontScale, thickness, &baseLine).width; }
    );

    return * max_element (
        linesWidths.cbegin(),
        linesWidths.cend()
    );
}
