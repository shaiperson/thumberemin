#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "config.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"
#include "TrackingInfo.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    VideoCapture capture;
    Screen* screen;
    string windowName;
    TrackingInfo* tracker;

    bool playingMode; // TODO make polymorphic with respect to run()?

    Mat captureAndPreprocessFrame();
    void switchToPlayingMode();
    bool keyOptions();

};

#endif
