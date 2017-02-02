#include "test.h"

// OK entonces parece que hay algo mal con la cuenta del phase. Porque tira errores. No debería.

void ContinuousSineWaveTest() {
    float freq1 = 220;
    float freq2 = 246.94;

    ContinuousSineWave csw(freq1);

    size_t wave1Size = 0.125 * (StaticConfiguration::sampleRate / freq1);
    size_t wave2Size = 0.125 * (StaticConfiguration::sampleRate / freq2);
    vector<float> wave1(wave1Size);
    vector<float> wave2(wave2Size);

    for (size_t i = 0; i < wave1Size; ++i)
        wave1.at(i) = csw.nextSample();

    csw.updateFrequency(freq2);

    for (size_t i = 0; i < wave2Size; ++i) {
        wave2.at(i) = csw.nextSample();
    }

    vector<float> wave(wave1.size() + wave2.size());

    auto copyiter = wave.begin();
    copy(wave1.cbegin(), wave1.cend(), copyiter);
    copy(wave2.cbegin(), wave2.cend(), copyiter);

    cout << wave << endl;
}
