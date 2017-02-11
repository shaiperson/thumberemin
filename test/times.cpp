#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

TEST_CASE("Histogram calculation, sequential", "[sequential],[histogram]") {
    Mat image = imread("ihtinput/rgbseq1.png");
    Mat hist = IHT_createHistArgumentShort();
    IHT_calc3DByteDepthUniformHist(&image, &hist);
    cout << "Time: " << timer::t << endl;
}
