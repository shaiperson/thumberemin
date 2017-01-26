#ifndef INITIALSCREEN_H
#define INITIALSCREEN_H

#include "Screen.h"

class InitialScreen : public Screen {
public:
    InitialScreen(VideoCapture& cap, string wn);
    void update();

private:
    void processFrame(Mat&);

    using Screen::capture;
    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    Rect samplingRegion;
    InstructionsText samplingInstructions;

    using Screen::dimRegions;
    using Screen::captureAndPreprocessFrame;
};

#endif
