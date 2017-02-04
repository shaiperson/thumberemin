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
    ContinuousSineWave csw(220, 10000);

    size_t cycleSize = csw.cycleSize();

    vector<float> wave;

    /* Read some samples */
    for (size_t i = 0; i < (15/12.0)*cycleSize; ++i) wave.push_back(csw.nextSample());

    /* Start with second wave */
    csw.updateFrequency(440);
    cerr << "Joining index " << wave.size()-1 << " with value " << wave[wave.size()-1] << endl;

    /* Read some more */
    for (size_t i = 0; i < (2/3.0)*cycleSize; ++i) wave.push_back(csw.nextSample());

    /* Check for NaNs */
    vector<pair<size_t, size_t>> nanranges = findNaNRanges(wave);

    cerr << "Ranges with NaNs: " << nanranges << endl;

    cout << wave << endl;
}

void traceWave(const vector<wholeWaveCase>& data) {
    ContinuousSineWave csw(data[0].freq1, StaticConfiguration::sampleRate);

    for (size_t d = 0; d < data.size(); ++d) {

        for (size_t i = 0; i < data.at(d).numSamples; ++i) {
            cout << csw.nextSample();
            if (i < data.at(d).numSamples - 1 || d < data.size()-1)
                cout << ",";
        }

        csw.updateFrequency(data.at(d).freq2);
    }

    cout << endl;
}
