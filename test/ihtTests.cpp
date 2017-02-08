/* In-house tracking tests ============================ */

#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

SCENARIO("Calculating the 3D histogram of an RGB 8-bit image") {
    GIVEN("A 5x5 RGB 8-bit image whose pixels are all [1,2,3]") {
        Mat image(5, 5, CV_8UC3, Scalar(1,2,3));
        Mat hist = IHT_createHistArgument();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(&image, &hist);

            // THEN("Histogram has 25, total number of image pixels, in [1,2,3] bin") {
            //     REQUIRE(hist.ref<float>(1,2,3) == 25); // float comparison by == is OK since these are simply counters
            // }
            //
            // THEN("And it has 0 in all other bins") {
            //     REQUIRE(hist.nzcount() == 1);
            // }
        }
    }
}
