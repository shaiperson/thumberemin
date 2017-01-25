#include "../include/Theremin.h"

Theremin::Theremin() :
    capture(DEFAULT_CAMERA),
    frameSize(
        capture.get(CV_CAP_PROP_FRAME_WIDTH),
        capture.get(CV_CAP_PROP_FRAME_HEIGHT)
    ),
    windowName("Theremin")
{
    if(!capture.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");

    screen = new InitialScreen(frameSize);
}

Theremin::~Theremin() {
    delete screen;
}

void Theremin::run() {
    Mat frame, frameCopy;
    while (true) {
        capture >> frameCopy;
        flip(frameCopy, frame, 1);

        screen->updateFrame(frame);

        imshow("camarita", frame);

        int key = waitKey(1);
        if (key == 113) {
            cout << "K, quitting." << endl;
            break;
        } else if (key == 13) {
            cout << "Entering playing mode" << endl;
            replaceScreen();
        }
    }
}

void Theremin::replaceScreen() {
    delete screen;
    screen = new PlayingScreen(frameSize);
}
