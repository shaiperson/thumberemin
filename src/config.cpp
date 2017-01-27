#include "../include/config.h"

DynamicConfiguration::DynamicConfiguration(const Size& fsz) {
    frameSize = Size(fsz);
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(10/12.0),
        0,
        frameSize.width/12.0,
        frameSize.height
    );
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };
    samplingRegion = Rect (
        playingRegion.x + 0.25*playingRegion.width,
        playingRegion.height*(10/12.0),
        playingRegion.width*0.5,
        playingRegion.width*0.5
    );
    trackingWindowSize = samplingRegion.size();
}

extern const int StaticConfiguation::defaultCamera = 0;
extern const string StaticConfiguation::gameWindowName = "Theremin";

DynamicConfiguration dynconf;
