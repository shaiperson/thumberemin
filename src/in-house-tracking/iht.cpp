/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone image e hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(const Mat* image, Mat* hist) {
    // eventualmente volar
    // CV_Assert(image->isContinuous());
    // CV_Assert(hist->isContinuous());

    unsigned char* imgdata = image->data;
    unsigned char* histdata = hist->data;
    unsigned char* pixel;

    size_t chs = 3; // should be == image->channels()

    size_t imgrows = image->rows;
    size_t imgcols = image->cols;

    size_t imgstep = image->step;
    size_t padding = imgstep - imgcols * chs * sizeof(uchar);

    size_t dimSize = 256;
    size_t planeSize = dimSize * dimSize;

    size_t i = 0;
    while (i < imgrows) {
        size_t j = 0;
        while (j < imgcols) {
            * (float*) (
                histdata +
                imgdata [0 * sizeof(uchar)] * planeSize * sizeof(float) +
                imgdata [1 * sizeof(uchar)] * dimSize * sizeof(float) +
                imgdata [2 * sizeof(uchar)] * sizeof(float)
            ) += 1;

            imgdata += chs * sizeof(uchar);
            j += 1;
        }
        imgdata += padding;
        i += 1;
    }
}

/* ================================================================= */

/* aux */

Mat IHT_createHistArgument() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_32FC1);
}
