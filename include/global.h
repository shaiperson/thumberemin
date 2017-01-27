#ifndef GLOBAL_H
#define GLOBAL_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <exception>
#include <vector>

using namespace cv;
using namespace std;

namespace config {
    extern int defaultCamera;
    extern string gameWindowName;
    extern Size frameSize;
    extern Rect playingRegion;
    extern vector<Rect> inactiveRegions;
    extern Rect samplingRegion;
    extern Size trackingWindowSize;
}

#endif
