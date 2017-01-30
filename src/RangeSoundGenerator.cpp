#include "../include/RangeSoundGenerator.h"

RangeSoundGenerator::RangeSoundGenerator() {
    float freq0 = StaticConfiguration::noteRange[0];
    float freq1 = StaticConfiguration::noteRange[1];

    float freq0log = log2(freq0);
    float freq1log = log2(freq1);
    float d = freq1log-freq0log;

    size_t pixelRow = dynconf.playingRegion.y;
    for (size_t i = 0; i < StaticConfiguration::totalNotes; ++i) {
        for (size_t counter = 0; counter < dynconf.pixelsPerNote; ++counter) {
            pixel2Freq[pixelRow++] = exp2(freq0log + ((float)i / StaticConfiguration::totalNotes));
        }
    }
}

void RangeSoundGenerator::update(const TrackingInfo& tracker) {

}
