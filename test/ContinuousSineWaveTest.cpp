#include "test.h"

// OK entonces parece que hay algo mal con la cuenta del phase. Porque tira errores. No debería.

void ContinuousSineWaveTest() {
    twoWaveCase case1 = {220, 246.94, 1, 1};
    cout << joinWaves(case1) << endl;
}

vector<float> joinWaves(const twoWaveCase& c) {
    ContinuousSineWave csw(c.freq1);

    cerr << c.freq1 << " " << c.freq2 << endl;

    vector<float> wave1(c.cuttingPoint1 * (StaticConfiguration::sampleRate / c.freq1));
    vector<float> wave2(c.cuttingPoint2 * (StaticConfiguration::sampleRate / c.freq2));

    for (size_t i = 0; i < wave1.size(); ++i)
        wave1.at(i) = csw.nextSample();

    cerr << wave1 << endl;

    csw.updateFrequency(c.freq2);

    for (size_t i = 0; i < wave2.size(); ++i)
        wave2.at(i) = csw.nextSample();

    vector<float> wave(wave1.size() + wave2.size());

    auto copyiter = wave.begin();
    copy(wave1.cbegin(), wave1.cend(), copyiter);
    copy(wave2.cbegin(), wave2.cend(), copyiter);

    return wave;
}
