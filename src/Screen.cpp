#include "../include/Screen.h"

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
    samplingInstructions (
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
