// #include "catch.hpp"
#include "exp.h"

int main() { return 0; }

void ContinuousSineWaveTest() {
    /* Cycle test */
    ContinuousSineWave csw(1.0/(4*M_PI), StaticConfiguration::sampleRate);

    size_t cycleSize = csw.cycleSize();

    vector<float> lol;

    /* Advance without reading */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) csw.nextSample();

    /* Read some */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) lol.push_back(csw.nextSample());

    /* Start with second wave */
    csw.updateFrequency(0.125*M_PI);
    cerr << "Joining point: " << lol.size()-1 << endl;

    /* Read some more */
    for (size_t i = 0; i < (1/3.0)*cycleSize; ++i) lol.push_back(csw.nextSample());

    /* Check for NaNs */
    vector<pair<size_t, size_t>> nanranges;
    for (size_t i = 0; i < lol.size(); ++i) {
        if (lol.at(i) != lol.at(i)) {
            pair<size_t, size_t> range;
            range.first = i;
            i++;
            while (i < lol.size() && lol.at(i) != lol.at(i))
                i++;
            range.second = i-1;
            nanranges.push_back(range);
        }
    }

    cerr << "Ranges with NaNs: " << nanranges << endl;

    cout << lol << endl;
}
