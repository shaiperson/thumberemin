#include "../include/InitialScreen.h"

// TODO hardcoded values
InitialScreen::InitialScreen() :
    samplingInstructions (
        {"Cover square", "with thumb", "and press 'enter'", "", "Press 'q' to quit"}
    ) {}

/* Derived-screen-specific additional frame processing for display*/
void InitialScreen::processFrame(Mat& frame, const TrackingInfo& tracker) const {
    // Initial screen in particular shows instructions and shit
    samplingInstructions.write(frame, (dynconf.samplingRegion).br() + Point(5,0));
}
