#include "../include/Tracker.h"

Tracker::Tracker(Mat& histogram) : sample(histogram) { }

void Tracker::update(const Mat& frame) { } // do actual tracking and update state accordingly
Point Tracker::current() const { return Point(0,0); } // returns position according to tracker state as updated by update()
Mat Tracker::takeSample(Mat& frame) { return Mat(); }
