/* In-house tracking tests ============================ */

#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

Mat image, hist_seq, hist_vec;

SCENARIO("Calculating the 3D histogram of an RGB 8-bit image with cols divisble by 5", "[unit], [hist]") {
    GIVEN("A 10x10 RGB 8-bit image whose pixels are all [1,2,3]") {
        Mat imageToCropFrom(13, 13, CV_8UC3, Scalar(1,2,3));
        image = imageToCropFrom(Rect(2,2,10,10)); // crop image so that it has padding
        hist_seq = IHT_createHistArgumentShort();
        hist_vec = IHT_createHistArgumentShort();

        WHEN("Histogram is calculated") {
            IHT_calc3DByteDepthUniformHist(image.data, hist_seq.data, image.rows, image.cols, image.step);
            IHT_calc3DByteDepthUniformHist_ASM(image.data, hist_vec.data, image.rows, image.cols, image.step);

            THEN("Histogram at [1,2,3] has 100, total number of image pixels") {
                REQUIRE(hist_seq.at<short>(3,2,1) == 100);
                REQUIRE(hist_vec.at<short>(3,2,1) == 100);
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
                    allOnes_seq = allOnes_seq && hist_seq.at<short>(i,0,0) == 1;

                REQUIRE(allOnes_seq);

                bool allOnes_vec = true;
                for (size_t i = 0; i < 25; ++i)
                    allOnes_vec = allOnes_vec && hist_vec.at<short>(i,0,0) == 1;

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

SCENARIO("Back-projecting an RGB histogram on an RGB 8-bit image", "[unit], [backproject]") {

    GIVEN("An RGB histogram with bin [1,2,3] at 10 and an image whose pixels are all [1,2,3]") {
        Mat imageToCropFrom(13, 13, CV_8UC3, Scalar(1,2,3));
        image = imageToCropFrom(Rect(2,2,10,10)); // crop image so that it has padding

        int histSizes[3] = {256, 256, 256};
        Mat hist(3, histSizes, CV_16UC1, Scalar(0));
        hist.at<short>(3,2,1) = 10;

        WHEN("Back-projected") {
            Mat backProjection_seq = IHT_createBackProjectArgumentShort(image.size());
            Mat backProjection_vec = IHT_createBackProjectArgumentShort(image.size());

            IHT_calc3DByteDepthBackProject(image.data, hist.data, backProjection_seq.data, image.rows, image.cols, image.step);
            IHT_calc3DByteDepthBackProject_ASM(image.data, hist.data, backProjection_vec.data, image.rows, image.cols, image.step);

            bool allTens;
            THEN("All pixels in sequential back projection have 10") {
                allTens = true;
                for (auto it = backProjection_seq.begin<short>(); it != backProjection_seq.end<short>(); ++it)
                    allTens = allTens && *it == 10;
                REQUIRE(allTens);
            }

            THEN("All pixels in vectorial back projection have 10") {
                allTens = true;
                for (auto it = backProjection_vec.begin<short>(); it != backProjection_vec.end<short>(); ++it)
                    allTens = allTens && *it == 10;
                REQUIRE(allTens);
            }
        }
    }

    GIVEN("RGB histogram with all [x,100,100] bins at 123, all [x,200,200] bins at 456, rest at 0; image with diagonal [1,100,100] and the rest [1,200,200]") {
        Mat imageToCropFrom(23, 23, CV_8UC3, Scalar(1,200,200));
        image = imageToCropFrom(Rect(2,2,20,20));
        for (size_t i = 0; i < 20; ++i)
            image.at<Vec3b>(i,i) = Vec3b(1, 100, 100);

        int histSizes[3] = {256, 256, 256};
        Mat hist(3, histSizes, CV_16UC1, Scalar(0));
        for (size_t i = 0; i < 255; ++i) {
            hist.at<short>(100, 100, i) = 123;
            hist.at<short>(200, 200, i) = 456;
        }

        WHEN("Back-projected") {
            Mat backProjection_seq = IHT_createBackProjectArgumentShort(image.size());
            Mat backProjection_vec = IHT_createBackProjectArgumentShort(image.size());

            IHT_calc3DByteDepthBackProject(image.data, hist.data, backProjection_seq.data, image.rows, image.cols, image.step);
            IHT_calc3DByteDepthBackProject_ASM(image.data, hist.data, backProjection_vec.data, image.rows, image.cols, image.step);

            bool allCorrectSeq, allCorrectVec;
            THEN("All diagonal pixels have 123") {
                allCorrectSeq = true;
                for (size_t i = 0; i < 20; ++i)
                    allCorrectSeq = allCorrectSeq && backProjection_seq.at<short>(i,i) == 123;
                REQUIRE(allCorrectSeq);

                allCorrectVec = true;
                for (size_t i = 0; i < 20; ++i)
                    allCorrectVec = allCorrectVec && backProjection_vec.at<short>(i,i) == 123;
                REQUIRE(allCorrectVec);
            }

            THEN("The rest has 456") {
                allCorrectSeq = true;
                for (size_t i = 0; i < 20; ++i)
                    for (size_t j = 0; j < 20; ++j)
                        if (i != j)
                            allCorrectSeq = allCorrectSeq && backProjection_seq.at<short>(i,j) == 456;
                REQUIRE(allCorrectSeq);

                allCorrectVec = true;
                for (size_t i = 0; i < 20; ++i)
                    for (size_t j = 0; j < 20; ++j)
                        if (i != j)
                            allCorrectVec = allCorrectVec && backProjection_vec.at<short>(i,j) == 456;
                REQUIRE(allCorrectVec);
            }
        }
    }
}

TEST_CASE("IHT image moments and centroid", "[moments], [unit]") {
    WHEN("Calculating moments for 3x3 all-1s matrix") {
        Mat mat(3, 3, CV_16UC1, Scalar(1));
        iht_moments ms(mat, Rect(0, 0, mat.cols, mat.rows));

        THEN("m00 is 9") {
            REQUIRE(ms.m00 == 9);
        }

        THEN("m10 is 9") {
            REQUIRE(ms.m10 == 9);
        }

        THEN("m01 is 9") {
            REQUIRE(ms.m01 == 9);
        }

        THEN("Centroid is middle point") {
            REQUIRE(ms.centroid == Point(1,1));
        }
    }

    WHEN("Calculating moments for a 3x3 1..9 matrix") {
        Mat mat_short(3, 3, CV_16UC1);
        for (auto it = mat_short.begin<short>(); it != mat_short.end<short>(); ++it)
            *it = it - mat_short.begin<short>() + 1;

        Mat mat_uchar;
        mat_short.convertTo(mat_uchar, CV_8UC1);

        // Calculate IHT moments+centroid and OpenCV's moments and induced centroid
        iht_moments iht_ms(mat_short, Rect(0,0,mat_short.cols, mat_short.rows));
        Moments cv_ms = moments(mat_uchar);

        THEN("All IHT zeroth and first moments equal OpenCV's") {
            REQUIRE(iht_ms.m00 == cv_ms.m00);
            REQUIRE(iht_ms.m10 == cv_ms.m10);
            REQUIRE(iht_ms.m01 == cv_ms.m01);
        }

        THEN("IHT centroid coincides with OpenCV-moments' induced centroid") {
            REQUIRE(iht_ms.centroid == Point(cv_ms.m10 / cv_ms.m00, cv_ms.m01 / cv_ms.m00));
        }

    }
}

TEST_CASE("Mean shift", "[meanshift], [unit]") {
    GIVEN("Various random matrices and windows") {
        srand(123);

        Mat image(100, 100, CV_16UC1);
        for (size_t i = 0; i < 100; ++i)
            for (size_t j = 0; j < 100; ++j)
                image.at<short>(i,j) = rand() % SHRT_MAX;

        Rect ihtAsmWindow(rand() % 70, rand() % 70, 10 + rand() % 20, 10 + rand() % 20);
        Rect ihtPtrsWindow(ihtAsmWindow);
        Rect ihtCvWindow(ihtAsmWindow);
        Rect cvWindow(ihtAsmWindow);

        WHEN("Some mean shift iterations computed using IHTs on the one hand and OpenCV on the other") {
            int iters = 37;

            IHT_meanShift_ASM(image.data, image.rows, image.cols, image.step, &ihtAsmWindow.x, &ihtAsmWindow.y, ihtAsmWindow.width, ihtAsmWindow.height, iters);
            IHT_meanShift(image.data, image.rows, image.cols, image.step, &ihtPtrsWindow.x, &ihtPtrsWindow.y, ihtPtrsWindow.width, ihtPtrsWindow.height, iters);
            IHT_meanShift_CV(image, ihtCvWindow, iters);
            
            meanShift(image, cvWindow, TermCriteria(TermCriteria::COUNT, iters, 0));

            THEN("IHT-CV and CV windows are shifted equally") {
                REQUIRE(ihtPtrsWindow == ihtCvWindow);
                REQUIRE(ihtPtrsWindow == cvWindow);
                REQUIRE(ihtCvWindow == cvWindow);
            }

            THEN("IHT-ASM and CV windows are shifted equally") {
                REQUIRE(ihtAsmWindow == cvWindow);
            }
        }
    }
}
