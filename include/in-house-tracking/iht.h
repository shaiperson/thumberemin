#ifndef IHT_H
#define IHT_H

#include "../global.h"

void IHT_calc3DByteDepthUniformHist(const Mat* image, Mat* hist);
void IHT_calc3DByteDepthBackProject(const Mat* image, const Mat* hist, Mat* result);

/* aux */
Mat IHT_createHistArgument();
Mat IHT_createBackProjectArgument(const Rect& rect);

#endif
