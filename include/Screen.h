#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "InstructionsText.h"

class Screen {
public:
    Screen(const Size& fsz, string wn);
    virtual ~Screen() { }
    void update(Mat& frame, const TrackingInfo& tracker);

protected:
    Size frameSize;
    string windowName;

    virtual void processFrame(Mat&, const TrackingInfo&) = 0;
    Mat captureAndPreprocessFrame();
    void dimRegions(Mat& frame, const vector<Rect> regions, double factor);
};

#endif
