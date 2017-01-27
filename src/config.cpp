#include "../include/config.h"

// int config::defaultCamera = 0;
// string config::gameWindowName = "Theremin";
// Size config::frameSize;
//
// Rect config::playingRegion = Rect ( // TODO hardcoded
//     config::frameSize.width*(10/12.0),
//     0,
//     config::frameSize.width/12.0,
//     config::frameSize.height
// );
//
// vector<Rect> config::inactiveRegions = {
//     Rect(0, 0, config::playingRegion.x, config::frameSize.height),
//     Rect(config::playingRegion.x+config::playingRegion.width, 0, config::frameSize.width-config::playingRegion.x-config::playingRegion.width-1, config::frameSize.height),
// };
//
// Rect config::samplingRegion = Rect (
//     config::playingRegion.x + 0.25*config::playingRegion.width,
//     config::playingRegion.height*(10/12.0),
//     config::playingRegion.width*0.5,
//     config::playingRegion.width*0.5
// );
//
// Size config::trackingWindowSize = samplingRegion.size();

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
