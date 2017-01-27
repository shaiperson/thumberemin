#ifndef INITIALSCREEN_H
#define INITIALSCREEN_H

#include "Screen.h"

class InitialScreen : public Screen {
public:
    InitialScreen();

private:
    void processFrame(Mat&, const TrackingInfo&) const;

    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    using Screen::dimRegions;

    InstructionsText samplingInstructions;
};

#endif
