#include "exp.h"

int main(int argc, char* argv[]) {
    // cswExp();
    ifstream f(argv[1]);
    vector<wholeWaveCase> data;

    float freq1, freq2;
    size_t n;
    while (f) {
        f >> freq1 >> freq2 >> n;
        wholeWaveCase c = {freq1, freq2, n};
        data.push_back(c);
    }

    traceWave(data);

    return 0;
}
