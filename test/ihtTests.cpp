/* In-house tracking tests ============================ */

#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

Mat image, hist_seq, hist_vec;

SCENARIO("Calculating the 3D histogram of an RGB 8-bit image with cols divisble by 5", "[hist]") {
    GIVEN("A 10x10 RGB 8-bit image whose pixels are all [1,2,3]") {
        Mat imageToCropFrom(13, 13, CV_8UC3, Scalar(1,2,3));
        image = imageToCropFrom(Rect(2,2,10,10)); // crop image so that it has padding
        hist_seq = IHT_createHistArgumentShort();
        hist_vec = IHT_createHistArgumentShort();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(image.data, hist_seq.data, image.rows, image.cols, image.step);
            IHT_calc3DByteDepthUniformHist_ASM(image.data, hist_vec.data, image.rows, image.cols, image.step);

            THEN("Histogram at [1,2,3] has 100, total number of image pixels") {
                REQUIRE(hist_seq.at<short>(1,2,3) == 100);
                REQUIRE(hist_vec.at<short>(1,2,3) == 100);
            }

            THEN("It has 0 in all other bins") {
                SparseMat sparseHist_seq(hist_seq); // use a sparse matrix for testing to avoid iterating through dense-matrix histogram of size 256^3 = 2^24
                SparseMat sparseHist_vec(hist_vec);
                REQUIRE(sparseHist_seq.nzcount() == 1);
                REQUIRE(sparseHist_vec.nzcount() == 1);
            }
        }
    }

    GIVEN("A 5x5 RGB 8-bit image with cols divisible by 5 whose pixels are (0, 0, 0..24)") {
        Mat imageToCropFrom(6, 6, CV_8UC3);
        image = imageToCropFrom(Rect(1,1,5,5)); // crop image so that it has padding
        for (auto it = image.begin<Vec3b>(); it != image.end<Vec3b>(); ++it)
            *it = Vec3b(0, 0, it - image.begin<Vec3b>());
        hist_seq = IHT_createHistArgumentShort();
        hist_vec = IHT_createHistArgumentShort();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(image.data, hist_seq.data, image.rows, image.cols, image.step);
            IHT_calc3DByteDepthUniformHist_ASM(image.data, hist_vec.data, image.rows, image.cols, image.step);

            THEN("Histogram has 1 in bins (0, 0, 0..24)") {
                bool allOnes_seq = true;
                for (size_t i = 0; i < 25; ++i)
                    allOnes_seq = allOnes_seq && hist_seq.at<short>(0,0,i) == 1;

                REQUIRE(allOnes_seq);

                bool allOnes_vec = true;
                for (size_t i = 0; i < 25; ++i)
                    allOnes_vec = allOnes_vec && hist_vec.at<short>(0,0,i) == 1;

                REQUIRE(allOnes_vec);
            }

            THEN("It has 0 in all other bins") {
                SparseMat sparseHist_seq(hist_seq); // use a sparse matrix for testing to avoid iterating through dense-matrix histogram of size 256^3 = 2^24
                SparseMat sparseHist_vec(hist_vec);
                REQUIRE(sparseHist_seq.nzcount() == 25);
                REQUIRE(sparseHist_vec.nzcount() == 25);
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
