#ifndef IHT_H
#define IHT_H

#include "../global.h"
#include "../../test/timer/timer.h"

void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist);

void IHT_calc3DByteDepthBackProject(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);

void IHT_meanShift_CV(const Mat& densityMap, Rect& window, size_t iters);

extern "C" {
    void IHT_calc3DByteDepthUniformHist_ASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthUniformHist_DISASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthBackProject_ASM(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);
}

/* aux */
Mat IHT_createHistArgumentFloat();
Mat IHT_createHistArgumentShort();
Mat IHT_createBackProjectArgumentFloat(const Size&);
Mat IHT_createBackProjectArgumentShort(const Size&);

struct iht_moments {
    float m00, m10, m01;
    Point centroid;
    iht_moments(const Mat& image, const Rect& window) : m00(0), m10(0), m01(0) {
        short curr;

        for (int x = window.tl().x; x < window.br().x; ++x) {
            for (int y = window.tl().y; y < window.br().y; ++y) {
                curr = image.at<short>(y,x);
                m00 += curr;
                m10 += x*curr;
                m01 += y*curr;
            }
        }

        // cout << "--------------------" << endl;
        // cout << "m00 " << m00 << endl;
        // cout << "m10 " << m10 << endl;
        // cout << "m01 " << m01 << endl;
        // cout << "--------------------" << endl;

        centroid = Point(round(m10/m00), round(m01/m00));
    }
};

#endif
