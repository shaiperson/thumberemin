#ifndef IHT_H
#define IHT_H

#include "../global.h"
#include "../timer/timer.h"

struct iht_moments {
    float m00, m10, m01;
    Point centroid;
    iht_moments(const Mat& image);
};

struct IHT_window {
    int x, y, width, height;
    Point tl() const { return Point(x,y); }
    Size size() const { return Size(width, height); }
    size_t total() const { return width*height; }

    IHT_window(const Rect& r) : x(r.x), y(r.y), width(r.width), height(r.height) { }
    IHT_window(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) { }
    IHT_window(const IHT_window& w) : x(w.x), y(w.y), width(w.width), height(w.height) { }
};

void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist);

void IHT_calc3DByteDepthBackProject(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);
void IHT_calc3DByteDepthBackProject_CV(const Mat& image, const Mat& hist, Mat& backProjection);

void IHT_meanShift(const uchar* densityMap, int maprows, int mapcols, int mapstep, IHT_window* w, size_t iters);
void IHT_meanShift_CV(const Mat& densityMap, IHT_window& window, size_t iters);

extern "C" {
    void IHT_calc3DByteDepthUniformHist_ASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthUniformHist_DISASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_calc3DByteDepthBackProject_ASM(const uchar* image, const uchar* hist, uchar* result, size_t imgrows, size_t imgcols, size_t imgstep);
    void IHT_meanShift_ASM(const uchar* densityMap, int maprows, int mapcols, int mapstep, IHT_window* w, size_t iters);
}

/* aux */
Mat IHT_createHistArgumentShort();
Mat IHT_createBackProjectArgumentShort(const Size&);

#endif
