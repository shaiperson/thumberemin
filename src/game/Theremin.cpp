#include "../../include/game/Theremin.h"

Theremin::Theremin() :
    capture(StaticConfiguration::defaultCamera)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize());

    screen = new InitialScreen;
    tracker = new ColorSampler;
}

Theremin::~Theremin() {
    delete screen;
    delete tracker;
    capture.release();
}

void Theremin::run() {
    Mat frame;
    int key = -1;

    while (key == -1) {
        capture >> frame;
        tracker->update(frame);
        screen->update(frame, *tracker);
        key = waitKey(1);
    }
}

void Theremin::runLive() {
    run();

    /* replace inital screen with playing screen */
    delete screen;
    screen = new PlayingScreen;

    /* take color sample from samplingRegion and replace ColorSampler with Tracker */
    Mat frameForSampling;
    capture >> frameForSampling;
    Mat sampleHistogram = tracker->takeSample(frameForSampling);

    delete tracker;
    tracker = new Tracker(sampleHistogram);

    run();
}
