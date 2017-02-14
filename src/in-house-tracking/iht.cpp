/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep) {

    GLOBAL_startTimer();

    size_t chs = 3; // CHANNELS

    size_t dimSize = 256;
    size_t planeSize = 65536;

    for (size_t i = 0; i < imgrows; ++i) {
        for (size_t j = 0; j < imgcols; ++j) {
            const uchar* pixel = imgdata + i*imgstep + j*chs;
            uchar* bin =
                histdata +
                pixel[0] * planeSize * sizeof(short) +
                pixel[1] * dimSize * sizeof(short) +
                pixel[2] * sizeof(short);
            *(short*)bin += 1;
        }
    }

    GLOBAL_stopTimer();

}

void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist) {

    GLOBAL_startTimer();

    for (size_t i = 0; i < image.rows; ++i) {
        for (size_t j = 0; j < image.cols; ++j) {
            const Vec3b& pixel = image.at<Vec3b>(i,j);
            hist.at<short>(pixel[0], pixel[1], pixel[2]) += 1;
        }
    }

    GLOBAL_stopTimer();

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
            * (short*) resdata = * (short*) (
                histdata +
                imgdata [0 * sizeof(uchar)] * planeSize * sizeof(short) +
                imgdata [1 * sizeof(uchar)] * dimSize * sizeof(short) +
                imgdata [2 * sizeof(uchar)] * sizeof(short)
            );
            imgdata += imgchs * sizeof(uchar);
            resdata += reschs * sizeof(short);
            j += 1;
        }
        imgdata += padding;
        i += 1;
    }
}

/* ================================================================= */

/* aux */

/* Consuming method requires 0-initialized */
Mat IHT_createHistArgumentFloat() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_32FC1, Scalar(0));
}

Mat IHT_createHistArgumentShort() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_16UC1, Scalar(0));
}

/* Consuming method requires 0-initialized */
Mat IHT_createBackProjectArgumentFloat(const Size& size) {
    return Mat(size, CV_32FC1, Scalar(0));
}

Mat IHT_createBackProjectArgumentShort(const Size& size) {
    return Mat(size, CV_16UC1, Scalar(0));
}
