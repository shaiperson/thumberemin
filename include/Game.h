#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "config.h"
#include "Capture.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"
#include "TrackingInfo.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    Capture capture;
    Screen* screen;
    string windowName;
    TrackingInfo* tracker;

    bool playingMode; // TODO make polymorphic with respect to run()?

    void switchToPlayingMode();
    bool keyOptions();

};

#endif
