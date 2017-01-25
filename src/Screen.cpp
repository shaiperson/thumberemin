#include "../include/Screen.h"

Screen::Screen(VideoCapture& cap, string wn) :
    windowName(wn),
    frameSize(
        cap.get(CV_CAP_PROP_FRAME_WIDTH),
        cap.get(CV_CAP_PROP_FRAME_HEIGHT)
    ),
    playingRegion (
        frameSize.width*(10/12.0),
        0,
        frameSize.width/12.0,
        frameSize.height
    )
{
    capture = cap; // try initializing in itialization list instead of here (gives -Wuninitialized)
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };
}

void Screen::update() {
    Mat frame = captureAndPreprocessFrame();
    processFrame(frame);
    imshow(windowName, frame);
}

void Screen::dimRegions(Mat& frame, const vector<Rect> regions, double factor) {
    Mat regionFrameData;
    for (const Rect& region : regions) {
        regionFrameData = frame(region);
        regionFrameData.convertTo(regionFrameData, -1, factor, 0);
    }
}

Mat Screen::captureAndPreprocessFrame() {
    Mat frame, frameCopy;
    capture >> frameCopy;
    flip(frameCopy, frame, 1);
    return frame;
}
