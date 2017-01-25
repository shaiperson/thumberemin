#include "../include/Screen.h"

PlayingScreen::PlayingScreen(const Size& size) {}

void PlayingScreen::updateFrame(Mat& frame) {
    frame.convertTo(frame, -1, 5, 0);
}
