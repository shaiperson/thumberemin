#include "../include/Screen.h"

Screen::Screen(const Size& fsz, const string& wn) :
    windowName(wn),
    frameSize(fsz),
    playingRegion ( // TODO hardcoded
        frameSize.width*(10/12.0),
        0,
        frameSize.width/12.0,
        frameSize.height
    )
{
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };
}

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
