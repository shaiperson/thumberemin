#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

#define REPETITIONS 300

TEST_CASE("Histogram calculation times", "[times],[sequential],[histogram]") {
    Mat image, hist;
    double time_its, time_ptrs, time_asm = 0;

    image = imread("../test/ihtinput/rgballover.png");

    for (size_t i = 0; i < REPETITIONS; ++i) {
        /* measure iterators version */
        hist = IHT_createHistArgumentShort();
        IHT_calc3DByteDepthUniformHist_CV(image, hist);
        time_its += timer::t;

        /* measure pointers version */
        hist = IHT_createHistArgumentShort();
        IHT_calc3DByteDepthUniformHist(image.data, hist.data, image.rows, image.cols, image.step);
        time_ptrs += timer::t;

        /* measure assembler version */
        hist = IHT_createHistArgumentShort();
        IHT_calc3DByteDepthUniformHist_ASM(image.data, hist.data, image.rows, image.cols, image.step);
        time_asm += timer::t;
    }

    cout << "C++ iterators ";
    cerr << time_its / REPETITIONS << endl;

    cout << "C++ pointers  ";
    cerr << time_ptrs / REPETITIONS << endl;

    cout << "Assembler SSE ";
    cerr << time_asm / REPETITIONS << endl;
}
