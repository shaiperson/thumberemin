#include "../include/Game.h"

Game::Game() :
    capture(StaticConfiguation::defaultCamera),
    windowName(StaticConfiguation::gameWindowName)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(Size(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT)));

    screen = new InitialScreen();
    tracker = new ColorSampler();
}

Game::~Game() {
    delete screen;
    delete tracker;
}

void Game::run() {
    /* wait for user to choose playing mode */
    while (keyOptions()) {
        Mat frame = captureAndPreprocessFrame();
        tracker->update(frame);
        screen->update(frame, *tracker);
    }

    if (playingMode) {
        while (keyOptions()) {
            Mat frame = captureAndPreprocessFrame();
            tracker->update(frame);
            screen->update(frame, *tracker);
        }
    }
}

bool Game::keyOptions() {
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

void Game::switchToPlayingMode() {
    delete screen;
    screen = new PlayingScreen();

    delete tracker;
    tracker = new Tracker();

    playingMode = true;
}

Mat Game::captureAndPreprocessFrame() {
    Mat frame, frameCopy;
    capture >> frameCopy;
    flip(frameCopy, frame, 1);
    return frame;
}
