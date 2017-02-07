/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone image e hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(Mat* image, SparseMat* hist) {
    // eventualmente volar
    // CV_Assert(image->isContinuous());
    // CV_Assert(hist->isContinuous());

    for (size_t i = 0; i < image->rows; ++i) {
        for (size_t j = 0; j < image->cols; ++j) {
            Vec3b& pixel = image->at<Vec3b>(i,j);
            *( (float*) ( hist->ptr(pixel[0], pixel[1], pixel[2], true) ) ) += 1;
        }
    }
}

/* ================================================================= */

/* aux */

SparseMat IHT_createHistArgument() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return SparseMat(3, histSize, CV_32FC1);
}
