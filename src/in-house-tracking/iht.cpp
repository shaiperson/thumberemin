/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(const Mat* image, Mat* hist) {
    unsigned char* imgdata = image->data;
    unsigned char* histdata = hist->data;

    size_t chs = 3; // should be == image->channels()

    size_t imgrows = image->rows;
    size_t imgcols = image->cols;

    size_t imgstep = image->step;
    size_t padding = imgstep - imgcols * chs * sizeof(uchar);

    size_t dimSize = 256;
    size_t planeSize = dimSize * dimSize;

    size_t i = 0;
    size_t j;
    while (i < imgrows) {
        j = 0;
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

/* RESULT SE SUPONE INICIALIZADA EN 0 */
void IHT_calc3DByteDepthBackProject(const Mat* image, const Mat* hist, Mat* result) {
    unsigned char* imgdata = image->data;
    unsigned char* histdata = hist->data;
    unsigned char* resdata = result->data;

    size_t imgchs = 3; // should be == image->channels()
    size_t reschs = 1;

    size_t imgrows = image->rows;
    size_t imgcols = image->cols;

    size_t imgstep = image->step;
    size_t padding = imgstep - imgcols * imgchs * sizeof(uchar);

    size_t dimSize = 256;
    size_t planeSize = dimSize * dimSize;

    size_t i = 0;
    size_t j;
    while (i < imgrows) {
        j = 0;
        while (j < imgcols) {
            * (float*) resdata = * (float*) (
                histdata +
                imgdata [0 * sizeof(uchar)] * planeSize * sizeof(float) +
                imgdata [1 * sizeof(uchar)] * dimSize * sizeof(float) +
                imgdata [2 * sizeof(uchar)] * sizeof(float)
            );
            imgdata += imgchs * sizeof(uchar);
            resdata += reschs * sizeof(float);
            j += 1;
        }
        imgdata += padding;
        i += 1;
    }
}

/* ================================================================= */

/* aux */

/* Consuming method requires 0-initialized */
Mat IHT_createHistArgument() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_32FC1, Scalar(0));
}

/* Consuming method requires 0-initialized */
Mat IHT_createBackProjectArgument(const Size& size) {
    return Mat(size, CV_32FC1, Scalar(0));
}
