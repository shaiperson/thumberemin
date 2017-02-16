#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

#define REPETITIONS 300

TEST_CASE("Histogram calculation times", "[times],[hist]") {
    Mat image, hist;
    double time_its, time_ptrs, time_asm, time_disasm = 0;

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

        /* measure O3 pointer-arithmetic disassembled version */
        hist = IHT_createHistArgumentShort();
        IHT_calc3DByteDepthUniformHist_DISASM(image.data, hist.data, image.rows, image.cols, image.step);
        time_disasm += timer::t;
    }

    cout << "C++ iterators   ";
    cerr << time_its / REPETITIONS << endl;

    cout << "C++ pointers    ";
    cerr << time_ptrs / REPETITIONS << endl;

    cout << "Assembler SSE   ";
    cerr << time_asm / REPETITIONS << endl;

    cout << "Disassembled O3 ";
    cerr << time_disasm / REPETITIONS << endl;

}

TEST_CASE("Back projection calculation times", "[times],[backproject]") {
    Mat image, hist, backproject;
    double time_ptrs, time_asm = 0;

    image = imread("../test/ihtinput/rgballover.png");

    for (size_t i = 0; i < REPETITIONS; ++i) {
        /* measure iterators version */
        hist = IHT_createHistArgumentShort();
        backproject = IHT_createBackProjectArgumentShort(image.size());
        IHT_calc3DByteDepthBackProject(image.data, hist.data, backproject.data, image.rows, image.cols, image.step);
        time_ptrs += timer::t;

        /* measure pointers version */
        hist = IHT_createHistArgumentShort();
        backproject = IHT_createBackProjectArgumentShort(image.size());
        IHT_calc3DByteDepthBackProject_ASM(image.data, hist.data, backproject.data, image.rows, image.cols, image.step);
        time_asm += timer::t;
    }

    cout << "C++ pointers    ";
    cerr << time_ptrs / REPETITIONS << endl;

    cout << "Assembler SSE   ";
    cerr << time_asm / REPETITIONS << endl;

}
