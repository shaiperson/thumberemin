#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "config.h"
#include "InstructionsText.h"
#include "TrackingInfo.h"

class Screen {
public:
    Screen();
    virtual ~Screen() { }
    void update(Mat& frame, const TrackingInfo& tracker);

protected:
    string windowName;
    Size frameSize;
    Rect playingRegion;
    vector<Rect> inactiveRegions;

    virtual void processFrame(Mat&, const TrackingInfo&) = 0;
    Mat captureAndPreprocessFrame();
    void dimRegions(Mat& frame, const vector<Rect> regions, double factor);
};

#endif
