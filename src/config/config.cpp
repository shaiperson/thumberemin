#include "../../include/config/config.h"

const int StaticConfiguration::defaultCamera = 0;
const string StaticConfiguration::gameWindowName = "Theremin";

const double StaticConfiguration::dimmingFactor = 0.7;
const double StaticConfiguration::screenDivisionFactor = 10;

const Size StaticConfiguration::trackingWindowSize = Size(50, 50);
const Scalar StaticConfiguration::samplingWindowColor = Scalar(255,191,0);
const int StaticConfiguration::samplingWindowThickness = 2;

const Scalar StaticConfiguration::trackingMarkerColor = Scalar(255,191,0);
const int StaticConfiguration::trackingMarkerThickness = 1;

DynamicConfiguration::DynamicConfiguration(const Size& fsz, uchar midiLow, int totalNotes) :
    frameSize(fsz), midiLow(midiLow), totalNotes(totalNotes)
{
    /* Calculate playing region height 1. leaving enough space for tracking marker 2. considering number of notes */
    size_t min_playingRegionMargin = StaticConfiguration::trackingWindowSize.height / 2;
    size_t max_playingRegionHeight = frameSize.height - 2*min_playingRegionMargin;

    pixelsPerNote = max_playingRegionHeight / totalNotes;
    size_t actual_playingRegionHeight = pixelsPerNote * totalNotes;

    size_t actual_playingRegionVerticalMargin = (frameSize.height - actual_playingRegionHeight)/2;

    /* Denotes the region of the screen where motion tracking happens */
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(8   / StaticConfiguration::screenDivisionFactor),
        actual_playingRegionVerticalMargin,
        StaticConfiguration::trackingWindowSize.width,
        actual_playingRegionHeight
    );

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
            actual_playingRegionVerticalMargin
        ),
        Rect ( // below playingRegion
            playingRegion.x, playingRegion.y + playingRegion.height,
            playingRegion.width,
            actual_playingRegionVerticalMargin
        )
    };

    /* Denotes the subregion of the playing region from where the color sample is taken for tracking */
    samplingRegion = Rect (
        playingRegion.x,
        playingRegion.height*(8/StaticConfiguration::screenDivisionFactor),
        StaticConfiguration::trackingWindowSize.width,
        StaticConfiguration::trackingWindowSize.height
    );

    /* Radius of circular tracking marker */
    trackingMarkerRadius = StaticConfiguration::trackingWindowSize.width / 2;

    /* Pixel row -> midi note */
    initializePixel2MidiNote();
}

void DynamicConfiguration::initializePixel2MidiNote() {

    uchar lo = midiLow;
    uchar hi = lo + totalNotes;

    size_t pixelRow = playingRegion.y + playingRegion.height - 1;
    for (uchar note = lo; note <= hi; ++note) {
        for (size_t counter = 0; counter < pixelsPerNote; ++counter) {
            pixel2MidiNote[pixelRow--] = note;
        }
    }
}

DynamicConfiguration dynconf;
