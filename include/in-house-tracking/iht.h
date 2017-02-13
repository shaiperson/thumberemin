#ifndef IHT_H
#define IHT_H

#include "../global.h"
#include "../../test/timer/timer.h"

void IHT_calc3DByteDepthUniformHist(const Mat* image, Mat* hist);
void IHT_calc3DByteDepthBackProject(const Mat* image, const Mat* hist, Mat* result);

extern "C" int IHT_calc3DByteDepthUniformHist_ASM(const Mat* image, Mat* hist);
extern "C" void IHT_calc3DByteDepthBackProject_ASM(const Mat* image, const Mat* hist, Mat* result);

/* aux */
Mat IHT_createHistArgumentFloat();
Mat IHT_createHistArgumentShort();
Mat IHT_createBackProjectArgumentFloat(const Size&);
Mat IHT_createBackProjectArgumentShort(const Size&);

#endif
