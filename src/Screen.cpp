#include "../include/Screen.h"

void Screen::update(Mat& frame, const TrackingInfo& tracker) const {
    /* darken inactive regions */
    dimRegions(frame, dynconf.inactiveRegions, StaticConfiguration::dimmingFactor);

    /* draw tracking window */
    drawRectangleWithCenterAndSize(frame, tracker.current(), tracker.windowSize());

    /* additional processing to be implemented by each derived screen class */
    processFrame(frame, tracker);

    /* show frame */
    imshow(StaticConfiguration::gameWindowName, frame);
}

void Screen::drawRectangleWithCenterAndSize(Mat& frame, const Point& center, const Size& size) const {
    Rect trackingWindow(center - Point(size.width/2, size.height/2), size);
    rectangle (
        frame,
        trackingWindow,
        StaticConfiguration::trackingWindowColor,
        StaticConfiguration::trackingWindowThickness
    );
}

void Screen::dimRegions(Mat& frame, const vector<Rect> regions, double factor) const {
    Mat regionFrameData;
    for (const Rect& region : regions) {
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, factor, 0);
    }
}
