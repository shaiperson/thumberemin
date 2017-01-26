#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    VideoCapture capture;
    Screen* screen;
    string windowName;

    bool playingMode; // TODO make polymorphic with respect to run()?

    void switchToPlayingMode();
    bool keyOptions();

};

#endif
