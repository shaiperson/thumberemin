#include "../include/in-house-tracking/iht.h"
#include "../include/timer/timer.h"
#include "times.h"

#include <iostream>
#include <vector>

int main( int argc, char* argv[] ) {
    vector<randomMeanshiftCase> cases(20);

    size_t iters = 100;
    for (auto it = cases.begin(); it != cases.end(); ++it) {

      IHT_meanShift_ASM(it->image.data, it->image.rows, it->image.cols, it->image.step, &it->ihtAsmWindow, iters);
      IHT_meanShift(it->image.data, it->image.rows, it->image.cols, it->image.step, &it->ihtPtrsWindow, iters);
      IHT_meanShift_CV(it->image, it->ihtCvWindow, iters);
      meanShift(it->image, it->cvWindow, TermCriteria(TermCriteria::COUNT, iters, 0));

      /* things */
    }
}
