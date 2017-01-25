#include "../include/Screen.h"

PlayingScreen::PlayingScreen(const Size& size) :
    playingRegion (
        size.width*(10/12.0),
        0,
        size.width/12.0,
        size.height
    )
{
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, size.height),
        Rect(playingRegion.x+playingRegion.width, 0, size.width-playingRegion.x-playingRegion.width-1, size.height),
    };
}

void PlayingScreen::updateFrame(Mat& frame) {
    dimRegions(frame, inactiveRegions, 0.3);
}
