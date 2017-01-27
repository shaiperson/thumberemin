#ifndef THEREMIN_H
#define THEREMIN_H

#include "global.h"
#include "config.h"
#include "Capture.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"
#include "TrackingInfo.h"
#include "ColorSampler.h"
#include "Tracker.h"

class Theremin {
public:
    Theremin();
    ~Theremin();
    void run();

private:
    Capture capture;
    Screen* screen;
    TrackingInfo* tracker;
    // SoundGenerator* sound;

    bool playingMode; // TODO make polymorphic with respect to run()?

    void switchToPlayingMode();
    bool keyOptions();

};

#endif
