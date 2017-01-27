#include "../include/global.h"

namespace config {
    int defaultCamera = 0;
    string gameWindowName = "Theremin";
    Size frameSize;

    Rect playingRegion ( // TODO hardcoded
        frameSize.width*(10/12.0),
        0,
        frameSize.width/12.0,
        frameSize.height
    );

    vector<Rect> inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };

    Rect samplingRegion (
        playingRegion.x + 0.25*playingRegion.width,
        playingRegion.height*(10/12.0),
        playingRegion.width*0.5,
        playingRegion.width*0.5
    );

    Size trackingWindowSize = samplingRegion.size();
}