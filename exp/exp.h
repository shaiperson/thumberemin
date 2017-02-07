#ifndef EXP_H
#define EXP_H

#include "../include/sound/ContinuousSineWave.h"
#include "../include/global.h"

#include <iomanip>

struct twoWaveCase {
    float freq1, freq2;
    float cuttingPoint1, cuttingPoint2;
};

void cswExp();
vector<pair<size_t, size_t>> findNaNRanges(const vector<float>& wave);

// twoWaveCase case1 = {220, 246.94, 1, 1}; // se rompe
// cout << joinWaves(case1) << endl;
//
// twoWaveCase case2 = {1, 1, 0, 2}; // se rompe para cuttingPoint1 0.325, para 0.335 no
// cout << joinWaves(case2) << endl;

// vector<float> joinWaves(const twoWaveCase& c) {
//     ContinuousSineWave csw(c.freq1);
//
//     vector<float> wave1(c.cuttingPoint1 * (StaticConfiguration::sampleRate / c.freq1));
//     vector<float> wave2(c.cuttingPoint2 * (StaticConfiguration::sampleRate / c.freq2));
//
//     for (size_t i = 0; i < wave1.size(); ++i)
//         wave1.at(i) = csw.nextSample();
//
//     csw.updateFrequency(c.freq2);
//
//     for (size_t i = 0; i < wave2.size(); ++i)
//         wave2.at(i) = csw.nextSample();
//
//     vector<float> wave(wave1.size() + wave2.size());
//
//     auto copyiter = wave.begin();
//     copy(wave1.cbegin(), wave1.cend(), copyiter);
//     copy(wave2.cbegin(), wave2.cend(), copyiter);
//
//     return wave;
// }

/* ============ AUX ============ */

template <typename T1, typename T2>
ostream& operator<<(ostream& o, const pair<T1,T2>& p) {
    o << "[" << p.first << "," << p.second << "]";
    return o;
}

template <typename T>
ostream& operator<<(ostream& o, const vector<T>& v) {
    if (v.size() == 0) {
        o << "[ ]";
    } else {
        for (auto const_it = v.cbegin(); const_it != v.cend()-1; ++const_it)
            o << *const_it << ",";
        o << *(v.cend()-1);
    }
    return o;
}

#endif
