#ifndef IHT_H
#define IHT_H

#include "../global.h"

void IHT_calc3DByteDepthUniformHist(Mat* image, SparseMat* hist);

/* aux */
SparseMat IHT_createHistArgument();

#endif
