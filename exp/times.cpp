#include "../include/in-house-tracking/iht.h"
#include "../include/timer/timer.h"
#include "times.h"

#include <iostream>
#include <vector>

#define REPETITIONS 1000
#define ITERS 10

int main( int argc, char* argv[] ) {
    if (argc < 2) {
        cerr << "Indicar experimento" << endl;
        return 1;
    }

    vector<SquareRandomSamplingCase> samplingCases = createSquareRandomSamplingCases(50, 100, 1);
    vector<SquareRandomBackprojCase> backprojCases = createSquareRandomBackprojCases(50, 100, 1);
    vector<SquareRandomMeanshiftCase> meanshiftCases = createSquareRandomMeanshiftCasesWithMidSquareWindow(500, 50, 100, 1);

    if (string("sampling") == argv[1]) {

        for (SquareRandomSamplingCase& c : samplingCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;
            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthUniformHist_CV(c.window, c.cvHist);
                cvTime += timer::t;
                IHT_calc3DByteDepthUniformHist(c.window.data, c.ptrsHist.data, c.window.rows, c.window.cols, c.window.step);
                ptrsTime += timer::t;
                IHT_calc3DByteDepthUniformHist_ASM(c.window.data, c.ptrsHist.data, c.window.rows, c.window.cols, c.window.step);
                asmTime += timer::t;
            }
            cout << cvTime / REPETITIONS << " " << ptrsTime / REPETITIONS << " " << asmTime / REPETITIONS << endl;
        }

    } else if (string("backprojection") == argv[1]) {

        for (SquareRandomBackprojCase& c : backprojCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;
            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthBackProject_CV(c.image, c.hist, c.cvBackproj);
                cvTime += timer::t;
                IHT_calc3DByteDepthBackProject(c.image.data, c.hist.data, c.ptrsBackproj.data, c.image.rows, c.image.cols, c.image.step);
                ptrsTime += timer::t;
                IHT_calc3DByteDepthBackProject_ASM(c.image.data, c.hist.data, c.ptrsBackproj.data, c.image.rows, c.image.cols, c.image.step);
                asmTime += timer::t;
            }
            cout << cvTime / REPETITIONS << " " << ptrsTime / REPETITIONS << " " << asmTime / REPETITIONS << endl;
        }

    } else if (string("meanshift") == argv[1]) {

        for (SquareRandomMeanshiftCase& c : meanshiftCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;
            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_meanShift_CV(c.densityMap, c.cvWindow, ITERS);
                cvTime += timer::t;
                IHT_meanShift(c.densityMap.data, c.densityMap.rows, c.densityMap.cols, c.densityMap.step, &c.ptrsWindow, ITERS);
                ptrsTime += timer::t;
                IHT_meanShift_ASM(c.densityMap.data, c.densityMap.rows, c.densityMap.cols, c.densityMap.step, &c.ptrsWindow, ITERS);
                asmTime += timer::t;
            }
            cout << cvTime / REPETITIONS << " " << ptrsTime / REPETITIONS << " " << asmTime / REPETITIONS << endl;
        }

    }

    return 0;
}
