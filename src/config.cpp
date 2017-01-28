#include "../include/config.h"

const int StaticConfiguation::defaultCamera = 0;
const string StaticConfiguation::gameWindowName = "Theremin";
const double StaticConfiguation::dimmingFactor = 0.7;
const Scalar StaticConfiguation::trackingWindowColor = Scalar(255,191,0);
const int StaticConfiguation::trackingWindowThickness = 2;

DynamicConfiguration::DynamicConfiguration(const Size& fsz) {
    frameSize = Size(fsz);
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(41/48.0),
        0,
        frameSize.width*(2/48.0),
        frameSize.height
    );
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };
    samplingRegion = Rect (
        playingRegion.x,
        playingRegion.height*(40/48.0),
        playingRegion.width,
        playingRegion.width
    );
    trackingWindowSize = samplingRegion.size();
}

DynamicConfiguration dynconf;
