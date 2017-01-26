#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "InstructionsText.h"

class Screen {
public:
    Screen(VideoCapture& cap, string wn);
    virtual ~Screen() {}
    void update();

protected:
    VideoCapture capture;
    Size frameSize;
    string windowName;
    Rect playingRegion;
    vector<Rect> inactiveRegions;

    virtual void processFrame(Mat&) = 0;
    Mat captureAndPreprocessFrame();
    void dimRegions(Mat& frame, const vector<Rect> regions, double factor);
};

#endif
