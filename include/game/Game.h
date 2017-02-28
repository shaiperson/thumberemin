#ifndef THEREMIN_H
#define THEREMIN_H

#include "../global.h"
#include "../config/config.h"
#include "../io/Capture.h"
#include "../gui/PlayingScreen.h"
#include "../gui/InitialScreen.h"
#include "../tracking/TrackingInfo.h"
#include "../tracking/ColorSampler.h"
#include "../tracking/Tracker.h"

class Game {
public:
    Game();
    Game(const string& mode);
    ~Game();
    void runFromVideoSource();
    void runLive();

private:
    Capture capture;
    Screen* screen;
    TrackingInfo* tracker;

    void run();

};

#endif
