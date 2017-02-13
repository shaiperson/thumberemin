#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

TEST_CASE("Histogram calculation, sequential", "[times],[sequential],[histogram]") {
    Mat image = imread("../test/ihtinput/rgballover.png");
    Mat hist = IHT_createHistArgumentShort();

    IHT_calc3DByteDepthUniformHist(image.data, hist.data, image.rows, image.cols, image.step);
    cerr << timer::t << endl;
}

TEST_CASE("Histogram calculation, vectorial", "[times],[vectorial],[histogram]") {
    Mat image = imread("../test/ihtinput/rgballover.png");
    Mat hist = IHT_createHistArgumentShort();

    IHT_calc3DByteDepthUniformHist_ASM(image.data, hist.data, image.rows, image.cols, image.step);
    cerr << timer::t << endl;
}
