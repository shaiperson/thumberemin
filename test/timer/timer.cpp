#include "timer.h"

high_resolution_clock::time_point timer::t0;
high_resolution_clock::time_point timer::t1;
double timer::t;

void timer::start() {
    timer::t0 = high_resolution_clock::now();
}

void timer::stop() {
    timer::t1 = high_resolution_clock::now();
    duration<double, ratio<1,1000000>> lapse =
        duration_cast<duration<double, ratio<1,1000000>>> ( timer::t1 - timer::t0 );
    timer::t = lapse.count();
}
