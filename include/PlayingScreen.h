#ifndef PLAYINGSCREEN_H
#define PLAYINGSCREEN_H

#include "Screen.h"

class PlayingScreen : public Screen {
public:
    PlayingScreen();
    using Screen::update; // hace falta?

private:
    void processFrame(Mat&, const TrackingInfo&);

    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    using Screen::dimRegions;
};

#endif
