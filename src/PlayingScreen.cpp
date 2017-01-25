#include "../include/Screen.h"

PlayingScreen::PlayingScreen(VideoCapture& cap, string wn) :
    Screen(cap, wn) {}

void PlayingScreen::update() {
    // dimRegions(frame, inactiveRegions, 0.3);
}
