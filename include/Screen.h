#ifndef SCREEN_H
#define SCREEN_H

#include "global.h"
#include "InstructionsText.h"

class Screen {
public:
    Screen(VideoCapture& cap, string wn);
    virtual ~Screen() {}
    void update();

protected:
    VideoCapture capture;
    Size frameSize;
    string windowName;
    Rect playingRegion;
    vector<Rect> inactiveRegions;

    virtual void processFrame(Mat&) = 0;
    Mat captureAndPreprocessFrame();
    void dimRegions(Mat& frame, const vector<Rect> regions, double factor);
};

class InitialScreen : public Screen {
public:
    InitialScreen(VideoCapture& cap, string wn);
    void update();

private:
    void processFrame(Mat&);

    using Screen::capture;
    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    Rect samplingRegion;
    InstructionsText samplingInstructions;

    using Screen::dimRegions;
    using Screen::captureAndPreprocessFrame;
};

class PlayingScreen : public Screen {
public:
    PlayingScreen(VideoCapture& cap, string windowName);
    void update();
private:
    void processFrame(Mat&);

    using Screen::capture;
    using Screen::frameSize;
    using Screen::windowName;
    using Screen::inactiveRegions;
    using Screen::playingRegion;

    using Screen::dimRegions;
    using Screen::captureAndPreprocessFrame;
};

#endif
