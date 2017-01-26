#include "../include/Theremin.h"

Theremin::Theremin() :
    capture(DEFAULT_CAMERA),
    windowName("Theremin")
{
    if(!capture.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");

    screen = new InitialScreen(capture, windowName);
}

Theremin::~Theremin() {
    delete screen;
}

void Theremin::run() {
    /* wait for user to choose playing mode */
    while (keyOptions()) {
        screen->update();
    }

    if (playingMode) {
        cout << "hola" << endl;
        while (keyOptions()) {
            screen->update();
        }
    }
}

bool Theremin::keyOptions() {
    int key = waitKey(1);
    bool continuePlaying = true;

    /* enter key */
    if (key == 13) {
        cout << "Entering playing mode" << endl;
        switchToPlayingMode();
        continuePlaying = false;
    }
    /* 'q' key */
    if (key == 113) {
        cout << "K, quitting." << endl;
        continuePlaying = false;
    }

    return continuePlaying;
}

void Theremin::switchToPlayingMode() {
    delete screen;
    screen = new PlayingScreen(capture, windowName);
    playingMode = true;
}
