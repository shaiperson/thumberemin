#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"

LOL ESTO

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

    Mat Screen::captureAndPreprocessFrame();
    void switchToPlayingMode();
    bool keyOptions();

};

#endif
