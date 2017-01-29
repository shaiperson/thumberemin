#include "../include/Screen.h"

void Screen::update(Mat& frame, const TrackingInfo& tracker) const {
    /* darken inactive regions */
    dimRegions(frame, dynconf.inactiveRegions, StaticConfiguration::dimmingFactor);

    /* draw tracking window */
    Rect trackingWindow(tracker.current(), tracker.windowSize());
    rectangle (
        frame,
        trackingWindow,
        StaticConfiguration::trackingWindowColor,
        StaticConfiguration::trackingWindowThickness
    );

    /* additional processing to be implemented by each derived screen class */
    processFrame(frame, tracker);

    /* show frame */
    imshow(StaticConfiguration::gameWindowName, frame);
}

void Screen::dimRegions(Mat& frame, const vector<Rect> regions, double factor) const {
    Mat regionFrameData;
    for (const Rect& region : regions) {
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, factor, 0);
    }
}
