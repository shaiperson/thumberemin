#ifndef SCREEN_H
#define SCREEN_H

#include "../global.h"
#include "../config/config.h"
#include "InstructionsText.h"
#include "../tracking/TrackingInfo.h"

class Screen {
public:
    virtual ~Screen() { }
    void update(Mat& frame, const TrackingInfo& tracker) const;

protected:
    virtual void processFrame(Mat& frame, const TrackingInfo& tracker) const = 0;
};

#endif
