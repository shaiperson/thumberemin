#include "../include/PlayingScreen.h"

/* Derived-screen-specific additional frame processing for display*/
void PlayingScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    drawNoteMarker (
        frame,
        tracker.current()
    );
}

void PlayingScreen::drawNoteMarker(Mat& frame, const Point& center) const {
    circle (
        frame,
        center,
        StaticConfiguration::trackingMarkerRadius,
        StaticConfiguration::trackingMarkerColor,
        StaticConfiguration::trackingMarkerThickness,
        CV_AA
    );
}
