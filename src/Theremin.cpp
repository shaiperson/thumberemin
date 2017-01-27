#include "../include/Theremin.h"

Theremin::Theremin() :
    capture(StaticConfiguation::defaultCamera)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize());

    screen = new InitialScreen();
    tracker = new ColorSampler();
}

Theremin::~Theremin() {
    delete screen;
    delete tracker;
}

void Theremin::run() {
    /* wait for user to choose playing mode */
    Mat frame;
    while (keyOptions()) {
        capture >> frame;
        tracker->update(frame);
        screen->update(frame, *tracker);
        // sound->update(*tracker);
    }

    if (playingMode) {
        while (keyOptions()) {
            capture >> frame;
            tracker->update(frame);
            screen->update(frame, *tracker);
            // sound->update(*tracker);
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
    screen = new PlayingScreen();

    delete tracker;
    tracker = new Tracker();

    playingMode = true;
}
