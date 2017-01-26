#include "../include/PlayingScreen.h"

PlayingScreen::PlayingScreen(VideoCapture& cap, string wn) :
    Screen(cap, wn) {}

void PlayingScreen::processFrame(Mat& frame) {
    dimRegions(frame, inactiveRegions, 0.3);
}
