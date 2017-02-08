/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone image e hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(Mat* image, Mat* hist) {
    // eventualmente volar
    // CV_Assert(image->isContinuous());
    // CV_Assert(hist->isContinuous());

    for (size_t i = 0; i < image->rows; ++i) {
        for (size_t j = 0; j < image->cols; ++j) {
            Vec3b& pixel = image->at<Vec3b>(i,j);
            hist->at<float>(pixel[0], pixel[1], pixel[2]) += 1;
        }
    }
}

/* ================================================================= */

/* aux */

Mat IHT_createHistArgument() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_32FC1);
}
