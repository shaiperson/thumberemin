#include "../include/initialization.h"

VideoCapture initializeVideoCapture() {
    VideoCapture cap(DEFAULT_CAMERA); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");
    return cap;
}

InstructionsText::InstructionsText(const vector<string>& lines, const Point& bottomLeft)
: lines(lines), bottomLeft(bottomLeft) {
    fontFace = FONT_HERSHEY_SIMPLEX;
    fontScale = 0.4;
    thickness = 1;
    firstLineSize = getTextSize(lines[0], fontFace, fontScale, thickness, &baseLine);
    color = Scalar(255,255,255);
}

void InstructionsText::write(Mat& frame) {
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

InitialScreen::InitialScreen(const Size& size) :
    activeRegion (
        size.width*(10/12.0),
        0,
        size.width/12.0,
        size.height
    ),
    samplingRegion (
        activeRegion.x + 0.25*activeRegion.width,
        activeRegion.height*(10/12.0),
        activeRegion.width*0.5,
        activeRegion.width*0.5
    ),
    samplingInstructions(
        {"Cover square", "with thumb", "and press 'enter'", "", "Press 'q' to quit"},
        Point(activeRegion.x, samplingRegion.y)

    )
{
    inactiveRegions = {
        Rect(0, 0, activeRegion.x, size.height),
        Rect(activeRegion.x+activeRegion.width, 0, size.width-activeRegion.x-activeRegion.width-1, size.height),
    };
}

void InitialScreen::updateFrame(Mat& frame) {
    /* darken inactive regions */
    Mat regionFrameData;
    for (Rect& region : inactiveRegions) {
        // cout << region << endl;
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, 0.3, 0); // TODO hardcoded values
    }

    /* draw rectangle marking finger color sampling region */
    rectangle(frame, samplingRegion, Scalar(255,191,0), 4); // TODO hardcoded values

    /* sampling instruction text */
    samplingInstructions.write(frame);
}
