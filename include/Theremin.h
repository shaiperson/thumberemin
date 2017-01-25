#ifndef THEREMIN_H
#define THEREMIN_H

#include "global.h"
#include "Screen.h"

class Theremin {
public:
    Theremin();
    void run();
private:
    VideoCapture capture;
    Size frameSize;

    // Screen* screen; // TODO make polymorphic

    InitialScreen initialScreen;
    PlayingScreen playingScreen;
    string windowName;

    void runner(Screen&);

    void runInitial();
    void runPlaying();
};

#endif
