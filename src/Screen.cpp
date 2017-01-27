#include "../include/Screen.h"

Screen::Screen() :
    windowName(StaticConfiguation::gameWindowName),
    frameSize(dynconf.frameSize),
    playingRegion(dynconf.playingRegion),
    inactiveRegions(dynconf.inactiveRegions) { }

void Screen::update(Mat& frame, const TrackingInfo& tracker) {
    /* darken inactive regions */
    dimRegions(frame, inactiveRegions, 0.3);

    /* draw tracking window */
    Rect trackingWindow(tracker.current(), tracker.windowSize());
    rectangle(frame, trackingWindow, Scalar(255,191,0), 4); // TODO hardcoded values (color and thickness)

    /* additional processing to be implemented by each derived screen class */
    processFrame(frame, tracker);

    /* show frame */
    imshow(windowName, frame);
}

void Screen::dimRegions(Mat& frame, const vector<Rect> regions, double factor) {
    Mat regionFrameData;
    for (const Rect& region : regions) {
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, factor, 0);
    }
}
