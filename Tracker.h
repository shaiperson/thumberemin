#ifndef TRACKER_H
#define TRACKER_H

class Tracker : public TrackingInfo {
public:
    Tracker(Mat& histogram) : sample(histogram);
    /* TODO */ void update(const Mat& frame) {} // do actual tracking and update state accordingly
    /* TODO */ Point current() const { return Point(0,0); } // returns position according to tracker state as updated by update()
private:
    Mat sample;
};

#endif
