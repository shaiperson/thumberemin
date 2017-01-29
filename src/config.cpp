#include "../include/config.h"

const int StaticConfiguration::defaultCamera = 0;
const string StaticConfiguration::gameWindowName = "Theremin";
const double StaticConfiguration::dimmingFactor = 0.7;
const Scalar StaticConfiguration::trackingWindowColor = Scalar(255,191,0);
const int StaticConfiguration::trackingWindowThickness = 2;
const Scalar StaticConfiguration::trackingMarkerColor = Scalar(255,191,0);
const int StaticConfiguration::trackingMarkerThickness = 1;
const float StaticConfiguration::noteRange[2] = {220, 880};
const size_t StaticConfiguration::totalNotes = 100;

DynamicConfiguration::DynamicConfiguration(const Size& fsz) {
    /* Size of the frames fed by the camera in use */
    frameSize = Size(fsz);

    /* Mapping y-coordinate -> frequency */
    size_t pixelsPerNote = frameSize.height / StaticConfiguration::totalNotes;

    /* Denotes the region of the screen where motion tracking happens */
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(41/48.0),
        0,
        frameSize.width*(2/48.0),
        pixelsPerNote * StaticConfiguration::totalNotes
    );

    size_t playingRegionVertMargin = (frameSize.height - playingRegion.height)/2;
    playingRegion += Point(0, playingRegionVertMargin);

    /* A collection of rectangles denoting the rest of the screen */
    inactiveRegions = {
        Rect ( // left of playing region
            0, 0,
            playingRegion.x,
            frameSize.height
        ),
        Rect ( // right of playing region
            playingRegion.x+playingRegion.width, 0,
            frameSize.width-playingRegion.x-playingRegion.width-1,
            frameSize.height
        ),
        Rect ( // above playingRegion
            playingRegion.x, 0,
            playingRegion.width,
            playingRegionVertMargin
        ),
        Rect ( // below playingRegion
            playingRegion.x, playingRegion.y + playingRegion.height,
            playingRegion.width,
            playingRegionVertMargin
        )
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

    /* Radius of circular tracking marker */
    trackingMarkerRadius = samplingRegion.height/2;
}

DynamicConfiguration dynconf;
