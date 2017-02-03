// #include "catch.hpp"
#include "test.h"

// OK entonces parece que hay algo mal con la cuenta del phase. Porque tira errores. No debería.

// twoWaveCase case1 = {220, 246.94, 1, 1}; // se rompe
// cout << joinWaves(case1) << endl;
//
// twoWaveCase case2 = {1, 1, 0, 2}; // se rompe para cuttingPoint1 0.325, para 0.335 no
// cout << joinWaves(case2) << endl;

// TEST_CASE("Single wave cycles generated correctly") {
//     ContinuousSineWave csw(1/(2*M_PI));
//
//     SECTION("Cycle size is correct given sample rate") {
//         size_t cycleSize = csw.nextCycle().size();
//         REQUIRE(c)
//     }
// }

void ContinuousSineWaveTest() {
    /* Cycle test */
    ContinuousSineWave csw(1.0/(4*M_PI));

    size_t cycleSize = csw.nextCycle().size();

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
