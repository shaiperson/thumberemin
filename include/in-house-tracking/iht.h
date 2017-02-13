#ifndef IHT_H
#define IHT_H

#include "../global.h"
#include "../../test/timer/timer.h"

void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
void IHT_calc3DByteDepthBackProject(const Mat* image, const Mat* hist, Mat* result);

extern "C" void IHT_calc3DByteDepthUniformHist_ASM(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep);
extern "C" void IHT_calc3DByteDepthBackProject_ASM(const Mat* image, const Mat* hist, Mat* result);

/* aux */
Mat IHT_createHistArgumentFloat();
Mat IHT_createHistArgumentShort();
Mat IHT_createBackProjectArgumentFloat(const Size&);
Mat IHT_createBackProjectArgumentShort(const Size&);

#endif
