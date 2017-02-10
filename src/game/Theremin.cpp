#include "../../include/game/Theremin.h"

Theremin::Theremin() :
    capture(StaticConfiguration::defaultCamera)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize());

    screen = new InitialScreen;
    tracker = new ColorSampler;
    sound = new SilentSoundGenerator;
}

Theremin::Theremin(const string& vid) :
    capture(vid)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing video");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize());

    screen = new InitialScreen;
    tracker = new ColorSampler;
    sound = new SilentSoundGenerator;
}

Theremin::~Theremin() {
    delete screen;
    delete tracker;
    capture.release();
}

void Theremin::runFromVideoSource() {
    switchToPlayingMode();

    Mat frame;
    while (capture.read(frame)) {
        // tracker->update(frame);
        // screen->update(frame, *tracker);
        // sound->update(*tracker);
        imshow("LOL", frame);
    }
}

void Theremin::runLive() {
    /* wait for user to choose playing mode */
    Mat frame;
    while (keyOptions()) {
        capture >> frame;
        tracker->update(frame);
        screen->update(frame, *tracker);
        sound->update(*tracker);
    }

    if (playingMode) {
        while (keyOptions()) {
            capture >> frame;
            tracker->update(frame);
            screen->update(frame, *tracker);
            sound->update(*tracker);
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
    /* set private playingMode flag */
    playingMode = true;

    /* replace inital screen with playing screen */
    delete screen;
    screen = new PlayingScreen;

    /* take color sample from samplingRegion and replace ColorSampler with Tracker */
    Mat frameForSampling;
    capture >> frameForSampling;
    Mat sampleHistogram = tracker->takeSample(frameForSampling);

    delete tracker;
    tracker = new Tracker(sampleHistogram);

    /* replace silent sound generator with range sound generator */
    delete sound;
    sound = new RangeSoundGenerator;
}
