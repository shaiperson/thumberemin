#include "../include/Game.h"

Game::Game() :
    capture(DEFAULT_CAMERA),
    windowName(GAME_WINDOW_NAME)
{
    if(!capture.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");

    screen = new InitialScreen(capture, windowName);
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
        tracker->update(frame)
        screen->update(frame, tracker);
    }

    if (playingMode) {
        while (keyOptions()) {
            Mat frame = captureAndPreprocessFrame();
            tracker->update(frame)
            screen->update(frame, tracker);
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
    screen = new PlayingScreen(capture, windowName);

    delete tracker;
    tracker = new PlayingTracker();

    playingMode = true;
}

Mat Game::captureAndPreprocessFrame() {
    Mat frame, frameCopy;
    capture >> frameCopy;
    flip(frameCopy, frame, 1);
    return frame;
}
