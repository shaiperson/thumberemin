#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "InstructionsText.h"

class Screen {
public:
    virtual ~Screen() {}
    virtual void updateFrame(Mat& frame) = 0;
protected:
    void dimRegions(Mat& frame, const vector<Rect> regions, double factor) {
        Mat regionFrameData;
        for (const Rect& region : regions) {
            regionFrameData = frame(region);
            regionFrameData.convertTo(regionFrameData, -1, factor, 0);
        }
    };
};

class InitialScreen : public Screen {
public:
    InitialScreen(const Size& size);
    void updateFrame(Mat& frame);

private:
    using Screen::dimRegions;

    vector<Rect> inactiveRegions;
    Rect activeRegion;
    Rect samplingRegion;
    InstructionsText samplingInstructions;

};

class PlayingScreen : public Screen {
public:
    PlayingScreen(const Size& size);
    void updateFrame(Mat& frame);
private:
    using Screen::dimRegions;

    vector<Rect> inactiveRegions;
    Rect playingRegion;
};

#endif
