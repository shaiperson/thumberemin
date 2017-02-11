#ifndef TIME_H
#define TIME_H

#include <chrono>

using namespace std;
using namespace chrono;

namespace timer {
    extern high_resolution_clock::time_point t0;
    extern high_resolution_clock::time_point t1;
    extern double t;
}

namespace timer {
    void start();
    void stop();
}

#endif
