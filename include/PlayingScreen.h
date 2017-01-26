#ifndef PLAYINGSCREEN_H
#define PLAYINGSCREEN_H

#include "Screen.h"

class PlayingScreen : public Screen {
public:
    PlayingScreen(VideoCapture& cap, string windowName);
    void update();
private:
    void processFrame(Mat&);

    using Screen::capture;
    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    using Screen::dimRegions;
    using Screen::captureAndPreprocessFrame;
};

#endif
