#ifndef GLOBAL_H
#define GLOBAL_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <exception>

using namespace cv;
using namespace std;

namespace config {
    int defaultCamera = 0;
    string gameWindowName = "Theremin";
    Size frameSize;

    playingRegion ( // TODO hardcoded
        frameSize.width*(10/12.0),
        0,
        frameSize.width/12.0,
        frameSize.height
    )

    inactiveRegions = {
        Rect(0, 0, playingRegion.x, frameSize.height),
        Rect(playingRegion.x+playingRegion.width, 0, frameSize.width-playingRegion.x-playingRegion.width-1, frameSize.height),
    };
}

#endif
