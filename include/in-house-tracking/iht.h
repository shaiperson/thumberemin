#ifndef IHT_H
#define IHT_H

#include "../global.h"

void IHT_calc3DByteDepthUniformHist(Mat* image, Mat* hist);

/* aux */
Mat IHT_createHistArgument();

#endif
