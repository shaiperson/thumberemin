#include "../include/PlayingScreen.h"

PlayingScreen::PlayingScreen() :
    Screen() {}

/* Derived-screen-specific additional frame processing for display*/
void PlayingScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    // no playing-screen-specific additional frame processing to do for now
}
