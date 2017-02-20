#ifndef IHT_H
#define IHT_H

#include "../global.h"
#include "../../test/timer/timer.h"

void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist);

void IHT_calc3DByteDepthBackProject(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);

void IHT_meanShift(const uchar* densityMap, int maprows, int mapcols, int mapstep, int* w_x, int* w_y, int width, int height, int iters);
void IHT_meanShift_CV(const Mat& densityMap, Rect& window, size_t iters);

extern "C" {
    void IHT_calc3DByteDepthUniformHist_ASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthUniformHist_DISASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthBackProject_ASM(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_meanShift_ASM(const uchar* densityMap, int maprows, int mapcols, int mapstep, int* w_x, int* w_y, int width, int height, int iters);
}

/* aux */
Mat IHT_createHistArgumentFloat();
Mat IHT_createHistArgumentShort();
Mat IHT_createBackProjectArgumentFloat(const Size&);
Mat IHT_createBackProjectArgumentShort(const Size&);

struct iht_moments {
    float m00, m10, m01;
    Point centroid;
    iht_moments(const Mat& image, const Rect& window);
};

#endif
