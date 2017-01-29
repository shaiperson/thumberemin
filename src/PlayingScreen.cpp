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
        dynconf.trackingMarkerRadius * (2/3.0),
        StaticConfiguration::trackingMarkerColor,
        10,
        CV_AA
    );

    line (
        frame,
        center,
        center-Point(dynconf.trackingMarkerRadius * 2, 0),
        StaticConfiguration::trackingMarkerColor,
        1,
        CV_AA
    );

    circle (
        frame,
        center,
        dynconf.trackingMarkerRadius * (1/12.0),
        StaticConfiguration::trackingMarkerColor,
        10,
        CV_AA
    );
    circle (
        frame,
        center,
        dynconf.trackingMarkerRadius,
        StaticConfiguration::trackingMarkerColor,
        StaticConfiguration::trackingMarkerThickness,
        CV_AA
    );
}
