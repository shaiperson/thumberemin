#include "../include/config.h"

const int StaticConfiguration::defaultCamera = 0;
const string StaticConfiguration::gameWindowName = "Theremin";

const double StaticConfiguration::dimmingFactor = 0.7;
const double StaticConfiguration::screenDivisionFactor = 48.0;

const Size StaticConfiguration::trackingWindowSize = Size(50, 50);
const Scalar StaticConfiguration::samplingWindowColor = Scalar(255,191,0);
const int StaticConfiguration::samplingWindowThickness = 2;

const Scalar StaticConfiguration::trackingMarkerColor = Scalar(255,191,0);
const int StaticConfiguration::trackingMarkerThickness = 1;

const Scalar StaticConfiguration::keyboardContourColor = Scalar(255,255,255);
const int StaticConfiguration::keyboardContourThickness = 2;

const float StaticConfiguration::noteRange[2] = {261.63, 1046.50};
const size_t StaticConfiguration::totalNotes = 24;

const size_t StaticConfiguration::sampleRate = 44000;
const size_t StaticConfiguration::numberOfChannels = 1;

DynamicConfiguration::DynamicConfiguration(const Size& fsz) {
    /* Size of the frames fed by the camera in use */
    frameSize = Size(fsz);

    /* Calculate playing region height 1. leaving enough space for tracking marker 2. considering number of notes */
    size_t min_playingRegionMargin = StaticConfiguration::trackingWindowSize.height / 2;
    size_t max_playingRegionHeight = frameSize.height - 2*min_playingRegionMargin;

    pixelsPerNote = max_playingRegionHeight / StaticConfiguration::totalNotes;
    size_t actual_playingRegionHeight = pixelsPerNote * StaticConfiguration::totalNotes;

    size_t actual_playingRegionVerticalMargin = (frameSize.height - actual_playingRegionHeight)/2;

    /* Denotes the region of the screen where motion tracking happens */
    playingRegion = Rect ( // TODO hardcoded
        frameSize.width*(41 / StaticConfiguration::screenDivisionFactor),
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
        playingRegion.height*(40/StaticConfiguration::screenDivisionFactor),
        StaticConfiguration::trackingWindowSize.width,
        StaticConfiguration::trackingWindowSize.height
    );

    /* Radius of circular tracking marker */
    trackingMarkerRadius = StaticConfiguration::trackingWindowSize.width / 2;

    /* Pixel row -> frequency */
    initializePixel2Freq();

    /* Graphics: keyboard */
    initializeKeyboardRectangles();
}

void DynamicConfiguration::initializePixel2Freq() {
    float freq0 = StaticConfiguration::noteRange[0];
    float freq1 = StaticConfiguration::noteRange[1];

    float freq0log = log2(freq0);
    float freq1log = log2(freq1);
    float d = freq1log-freq0log;

    size_t pixelRow = playingRegion.y + playingRegion.height - 1;
    for (size_t i = 0; i < StaticConfiguration::totalNotes; ++i) {
        for (size_t counter = 0; counter < pixelsPerNote; ++counter) {
            pixel2Freq[pixelRow--] = exp2(freq0log + ((float)i / StaticConfiguration::totalNotes));
        }
    }
}

void DynamicConfiguration::initializeKeyboardRectangles() {
    Rect keyboardContour = playingRegion - Point(playingRegion.width*1.5, 0);
    whiteKeysRects.push_back(keyboardContour);

    size_t whiteKeyHeight = playingRegion.height / 15;
    size_t whiteKeyWidth = playingRegion.width;
    size_t currWhiteKeyBaseY = playingRegion.br().y - 1;

    for (size_t t = 0; t < 15; ++t) {
        whiteKeysRects.push_back (
            Rect (
                keyboardContour.x,
                currWhiteKeyBaseY-whiteKeyHeight,
                whiteKeyWidth,
                whiteKeyHeight
            )
        );
        currWhiteKeyBaseY -= whiteKeyHeight;
    }

    size_t blackKeyHeight = whiteKeyHeight*0.5;
    size_t blackKeyWidth = whiteKeyWidth/2;
    size_t currBlackKeyBaseY = playingRegion.br().y - 1 - whiteKeyHeight*(3/4.0);

    // index of white keys whose "sharp" is a black key on the keyboard
    set<int> whiteKeyIdsWithSharpBlackKey = {0,1,3,4,5,7,8,10,11,12};

    for (size_t i = 0; i < 15; ++i) {
        if (whiteKeyIdsWithSharpBlackKey.count(i)) {
            blackKeysRects.push_back (
                Rect (
                    keyboardContour.x,
                    currBlackKeyBaseY-blackKeyHeight,
                    blackKeyWidth,
                    blackKeyHeight
                )
            );
        }
        currBlackKeyBaseY -= whiteKeyHeight;
    }
}

DynamicConfiguration dynconf;
