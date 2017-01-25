#include "../include/Screen.h"

// TODO hardcoded values
InitialScreen::InitialScreen(VideoCapture& cap, string wn) :
    Screen(cap, wn),
    samplingRegion (
        playingRegion.x + 0.25*playingRegion.width,
        playingRegion.height*(10/12.0),
        playingRegion.width*0.5,
        playingRegion.width*0.5
    ),
    samplingInstructions (
        {"Cover square", "with thumb", "and press 'enter'", "", "Press 'q' to quit"},
        Point(playingRegion.x, samplingRegion.y)
    ) {}

void InitialScreen::update() {
    Mat frame = captureAndPreprocess();

    /* darken inactive regions */
    dimRegions(frame, inactiveRegions, 0.3);

    /* draw rectangle marking finger color sampling region */
    rectangle(frame, samplingRegion, Scalar(255,191,0), 4); // TODO hardcoded values

    /* sampling instruction text */
    samplingInstructions.write(frame);

    /* show frame*/
    imshow(windowName, frame);
}
