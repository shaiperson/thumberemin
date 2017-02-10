/* In-house tracking tests ============================ */

#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

SCENARIO("Calculating the 3D histogram of an RGB 8-bit image", "[hist]") {
    GIVEN("A 5x5 RGB 8-bit image whose pixels are all [1,2,3]") {
        Mat imageToCropFrom(6, 6, CV_8UC3, Scalar(1,2,3));
        Mat image = imageToCropFrom(Rect(1,1,5,5)); // crop image so that it is not continuous

        Mat hist = IHT_createHistArgument();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(&image, &hist);

            THEN("Histogram at [1,2,3] has 25, total number of image pixels") {
                REQUIRE(hist.at<float>(1,2,3) == 25); // float comparison by == is OK since these are simply counters
            }

            THEN("And it has 0 in all other bins") {
                SparseMat sparseHist(hist); // use a sparse matrix for testing to avoid iterating through dense-matrix histogram of size 256^3 = 2^24
                REQUIRE(sparseHist.nzcount() == 1);
            }
        }
    }
}
