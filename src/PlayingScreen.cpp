#include "../include/PlayingScreen.h"

/* Derived-screen-specific additional frame processing for display*/
void PlayingScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    drawNoteMarker (
        frame,
        tracker.current()
    );
}

void PlayingScreen::drawNoteMarker(Mat& frame, const Point& center) const {
    /* "FREQUENCY POINTER" */
    circle (
        frame,
        center,
        dynconf.trackingMarkerRadius * (1/48.0),
        StaticConfiguration::trackingMarkerColor,
        10,
        CV_AA
    );

    Point freqPoint(center - Point(dynconf.trackingMarkerRadius * 3, 0));
    line (
        frame,
        center,
        freqPoint,
        StaticConfiguration::trackingMarkerColor,
        1,
        CV_AA
    );

    /* print frequency */
    float freq = dynconf.pixel2Freq[center.y];
    stringstream stream;
    stream << fixed << setprecision(1) << freq;

    putText(frame, stream.str(), freqPoint - Point(0, 5), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255,255,255));

    /* ENCLOSING CIRCLES */
    circle ( // inner
        frame,
        center,
        dynconf.trackingMarkerRadius * (2/3.0),
        StaticConfiguration::trackingMarkerColor,
        10,
        CV_AA
    );

    circle ( // outer
        frame,
        center,
        dynconf.trackingMarkerRadius,
        StaticConfiguration::trackingMarkerColor,
        StaticConfiguration::trackingMarkerThickness,
        CV_AA
    );
}
