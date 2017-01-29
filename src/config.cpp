#include "../include/config.h"

const int StaticConfiguration::defaultCamera = 0;
const string StaticConfiguration::gameWindowName = "Theremin";
const double StaticConfiguration::dimmingFactor = 0.7;
const Scalar StaticConfiguration::trackingWindowColor = Scalar(255,191,0);
const int StaticConfiguration::trackingWindowThickness = 2;
const int StaticConfiguration::octaves = 2;

DynamicConfiguration::DynamicConfiguration(const Size& fsz) {
    /* Size of the frames fed by the camera in use */
    frameSize = Size(fsz);

    /* Denotes the region of the screen where motion tracking happens */
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(41/48.0),
        0,
        frameSize.width*(2/48.0),
        frameSize.height
    );

    /* A collection of rectangles denoting the rest of the screen */
    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };

    /* Denotes the subregion of the playing region from where the color sample is taken for tracking */
    samplingRegion = Rect (
        playingRegion.x,
        playingRegion.height*(40/48.0),
        playingRegion.width,
        playingRegion.width
    );

    /* Size of the sampling region */
    trackingWindowSize = samplingRegion.size();
}

DynamicConfiguration dynconf;
