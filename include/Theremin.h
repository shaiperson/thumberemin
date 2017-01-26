#ifndef THEREMIN_H
#define THEREMIN_H

#include "global.h"
#include "PlayingScreen.h"
#include "InitialScreen.h"

class Theremin {
public:
    Theremin();
    ~Theremin();
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
