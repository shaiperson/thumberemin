#ifndef PLAYINGSCREEN_H
#define PLAYINGSCREEN_H

#include "Screen.h"

class PlayingScreen : public Screen {
public:

private:
    void processFrame(Mat&, const TrackingInfo&) const;
    using Screen::dimRegions;
};

#endif
