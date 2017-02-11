/* In-house tracking tests ============================ */

#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

Mat image, hist;

SCENARIO("Calculating the 3D histogram of an RGB 8-bit image", "[hist]") {
    GIVEN("A 5x5 RGB 8-bit image whose pixels are all [1,2,3]") {
        Mat imageToCropFrom(6, 6, CV_8UC3, Scalar(1,2,3));
        image = imageToCropFrom(Rect(1,1,5,5)); // crop image so that it is not continuous
        hist = IHT_createHistArgumentShort();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(&image, &hist);

            THEN("Histogram at [1,2,3] has 25, total number of image pixels") {
                REQUIRE(hist.at<short>(1,2,3) == 25); // short comparison by == is OK since these are simply counters
            }

            THEN("And it has 0 in all other bins") {
                SparseMat sparseHist(hist); // use a sparse matrix for testing to avoid iterating through dense-matrix histogram of size 256^3 = 2^24
                REQUIRE(sparseHist.nzcount() == 1);
            }
        }
    }
}

SCENARIO("Back-projecting an RGB histogram on an RGB 8-bit image", "[backproject]") {
    GIVEN("An RGB histogram with bin [1,2,3] at 10 and an image whoe pixels are all [1,2,3]") {
        Mat image(5, 5, CV_8UC3, Scalar(1,2,3));

        int histSizes[3] = {256, 256, 256};
        Mat hist(3, histSizes, CV_16UC1, Scalar(0));
        hist.at<short>(1,2,3) = 10;

        WHEN("Back-projected") {
            Mat backProjection = IHT_createBackProjectArgumentShort(image.size());
            IHT_calc3DByteDepthBackProject(&image, &hist, &backProjection);

            THEN("All pixels in back projection have 10") {
                bool allTens = true;
                for (auto it = backProjection.begin<short>(); it != backProjection.end<short>(); ++it)
                    allTens = allTens && *it == 10;
                REQUIRE(allTens);
            }
        }
    }
}
