#include "../include/InitialScreen.h"

// TODO hardcoded values
InitialScreen::InitialScreen() :
    Screen(),
    samplingInstructions (
        {"Cover square", "with thumb", "and press 'enter'", "", "Press 'q' to quit"}
    ) {}

/* Derived-screen-specific additional frame processing for display*/
void InitialScreen::processFrame(Mat& frame, const TrackingInfo& tracker) {
    // Initial screen in particular shows instructions and shit
    samplingInstructions.write(frame, Point((dynconf.playingRegion).x, (tracker.current()).y));
}
