#ifndef TIME_H
#define TIME_H

#include <chrono>

using namespace std;
using namespace chrono;

namespace timer {
    extern high_resolution_clock::time_point t0;
    extern high_resolution_clock::time_point t1;
    extern double t;
    extern double sampling;
    extern double trackingAccum;
    extern size_t trackingRepetitions;
}

namespace timer {
    void start();
    void stop();
}

/* timer wrappers - declare with C linkage to make visible from .asm */
extern "C" {
    void GLOBAL_startTimer();
    void GLOBAL_stopTimer();
}

#endif
