#include "../../include/gui/PlayingScreen.h"

/* Derived-screen-specific additional frame processing for display*/
void PlayingScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    drawTrackingMarker (
        frame,
        tracker.current()
    );
}

void PlayingScreen::drawTrackingMarker(Mat& frame, const Point& center) const {
    /* "FREQUENCY POINTER" */
    circle (
        frame,
        center,
        dynconf.trackingMarkerRadius*0.25,
        StaticConfiguration::trackingMarkerColor,
        10,
        CV_AA
    );
}
