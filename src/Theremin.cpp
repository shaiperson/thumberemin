#include "../include/Theremin.h"

Theremin::Theremin() :
    capture(DEFAULT_CAMERA),
    frameSize(
        capture.get(CV_CAP_PROP_FRAME_WIDTH),
        capture.get(CV_CAP_PROP_FRAME_HEIGHT)
    ),
    initialScreen(frameSize),
    playingScreen(frameSize),
    windowName("Theremin")
{
    if(!capture.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");
}

void Theremin::run() {
    namedWindow(windowName);
    runInitial();
}

void Theremin::runInitial() {
    Mat frame, frameCopy;
    while (true) {
        capture >> frameCopy;
        flip(frameCopy, frame, 1);

        initialScreen.updateFrame(frame);

        imshow("camarita", frame);

        int key = waitKey(1);
        if (key == 113) {
            cout << "K, quitting." << endl;
            break;
        } else if (key == 13) {
            cout << "Entering playing mode" << endl;
            runPlaying();
            break;
        }
    }
}

void Theremin::runPlaying() {
    Mat frame, frameCopy;
    while (true) {
        capture >> frameCopy;
        flip(frameCopy, frame, 1);

        playingScreen.updateFrame(frame);

        imshow("camarita", frame);

        int key = waitKey(1);
        if (key == 113) {
            cout << "K, quitting." << endl;
            break;
        }
    }
}
