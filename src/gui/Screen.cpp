#include "../../include/gui/Screen.h"

void Screen::update(Mat& frame, const TrackingInfo& tracker) const {
    /* additional processing to be implemented by each derived screen class */
    processFrame(frame, tracker);
    /* show frame */
    imshow(StaticConfiguration::gameWindowName, frame);
}
