#include "initialization.h"

VideoCapture initializeVideoCapture() {
    VideoCapture cap(DEFAULT_CAMERA); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");
    return cap;
}

InitialScreen::InitialScreen(const Size& size) {
    double screenDivisionFactor = 12.0;
    activeRegion = Rect(
        size.width*(10/screenDivisionFactor),
        0,
        size.width/screenDivisionFactor,
        size.height
    );

    inactiveRegions = {
        Rect(0, 0, activeRegion.x, size.height),
        Rect(activeRegion.y, 0, size.width-activeRegion.x-activeRegion.width-1, size.height),
    };

    samplingRegion = Rect (
        activeRegion.x+(1/3.0)*activeRegion.width,
        activeRegion.height*(10/12.0),
        activeRegion.width*(1/3.0),
        activeRegion.width*(1/3.0)
    );
}

void InitialScreen::updateFrame(Mat& frame) {
    /* darken inactive regions */
    Mat regionFrameData;
    for (Rect& region : inactiveRegions) {
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, 0.3, 0); // TODO hardcoded values
    }
    // left = frame(screencfg.inactiveRegions[0]);
    // right = frame(screencfg.inactiveRegions[1]);
    // stripe = frame(screencfg.activeRegion);

    /* draw rectangle marking finger color sampling region */
    rectangle(frame, samplingRegion, Scalar(255,191,0), 4); // TODO hardcoded values

}
