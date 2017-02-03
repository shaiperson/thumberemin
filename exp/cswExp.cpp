// #include "catch.hpp"
#include "exp.h"

vector<pair<size_t, size_t>> findNaNRanges(const vector<float>& wave) {
    vector<pair<size_t, size_t>> result;
    for (size_t i = 0; i < wave.size(); ++i) {
        float curr = wave.at(i);
        if (curr != curr) {
            pair<size_t, size_t> range;
            range.first = i;
            i++;
            while (i < wave.size() && wave.at(i) != wave.at(i))
                i++;
            range.second = i-1;
            result.push_back(range);
        }
    }
    return result;
}

void cswExp() {
    /* Cycle test */
    ContinuousSineWave csw(1, 100);

    size_t cycleSize = csw.cycleSize();

    vector<float> wave;

    /* Advance without reading */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) csw.nextSample();

    /* Read some */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) wave.push_back(csw.nextSample());

    /* Start with second wave */
    csw.updateFrequency(2);
    cerr << "Joining point: " << wave.size()-1 << endl;

    /* Read some more */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) wave.push_back(csw.nextSample());

    /* Check for NaNs */
    vector<pair<size_t, size_t>> nanranges = findNaNRanges(wave);

    cerr << "Ranges with NaNs: " << nanranges << endl;

    cout << wave << endl;
}
