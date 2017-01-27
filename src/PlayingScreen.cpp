#include "../include/PlayingScreen.h"

PlayingScreen::PlayingScreen(const Size& fsz, const string& wn) :
    Screen() {}

/* Derived-screen-specific additional frame processing for display*/
void PlayingScreen::processFrame(Mat& frame, const TrackingInfo& tracker) {
    // no playing-screen-specific additional frame processing to do for now
}
