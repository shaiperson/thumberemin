#ifndef TEST_H
#define TEST_H

#include "../include/ContinuousSineWave.h"
#include "../include/global.h"

void ContinuousSineWaveTest();

struct twoWaveCase {
    float freq1, freq2;
    float cuttingPoint1, cuttingPoint2;
};

vector<float> joinWaves(const twoWaveCase&);

/* ============ AUX ============ */

template <typename T>
ostream& operator<<(ostream& o, const vector<T>& v) {
    // o << "[ ";
    for (auto const_it = v.cbegin(); const_it != v.cend()-1; ++const_it)
        o << *const_it << ",";
    o << *(v.cend()-1);
    // o << "]";
    return o;
}

#endif
