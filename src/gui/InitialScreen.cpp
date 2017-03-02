#include "../../include/gui/InitialScreen.h"

InitialScreen::InitialScreen() :
    samplingInstructions (
        StaticConfiguration::initialInstructionsLines
    ) { }

/* Derived-screen-specific additional frame processing for display*/
void InitialScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    drawRectangleWithCenterAndSize(frame, tracker.current(), tracker.windowSize());
    samplingInstructions.write(frame, (dynconf.samplingRegion).br() + Point(5,0));
}

void InitialScreen::drawRectangleWithCenterAndSize(Mat& frame, const Point& center, const Size& size) const {
    Rect trackingWindow(center - Point(size.width/2, size.height/2), size);
    rectangle (
        frame,
        trackingWindow,
        StaticConfiguration::samplingWindowColor,
        StaticConfiguration::samplingWindowThickness
    );
}
