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

    vector<samplingCase> samplingCases = createsamplingCases(50, 150, 5);
    vector<backprojCase> backprojCases = createbackprojCases(50, 150, 5);
    vector<meanshiftCase> meanshiftCases = createmeanshiftCasesWithMidSquareWindow(500, 52, 132, 4);

    if (string("sampling") == argv[1]) {

        for (samplingCase& c : samplingCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthUniformHist_CV(c.window, c.cvHist);
                cvTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthUniformHist(c.window.data, c.ptrsHist.data, c.window.rows, c.window.cols, c.window.step);
                ptrsTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthUniformHist_ASM(c.window.data, c.ptrsHist.data, c.window.rows, c.window.cols, c.window.step);
                asmTime += timer::t;
            }

            c.cvTime = cvTime / REPETITIONS;
            c.ptrsTime = ptrsTime / REPETITIONS;
            c.asmTime = asmTime / REPETITIONS;
        }

        cout << endl << "(área de V, microsegundos)" << endl;
        cout << endl;
        cout << "Versión idiomática" << endl;
        // format print idiomatic
        for (samplingCase& c : samplingCases) {
            cout << "(" << c.window.total() << ", " << c.cvTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión con punteros" << endl;
        // format print pointers
        for (samplingCase& c : samplingCases) {
            cout << "(" << c.window.total() << ", " << c.ptrsTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión vectorial" << endl;
        // format print asm
        for (samplingCase& c : samplingCases) {
            cout << "(" << c.window.total() << ", " << c.asmTime << ")" << endl;
        }

    } else if (string("backprojection") == argv[1]) {

        for (backprojCase& c : backprojCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthBackProject_CV(c.frame, c.hist, c.cvBackproj);
                cvTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthBackProject(c.frame.data, c.hist.data, c.ptrsBackproj.data, c.frame.rows, c.frame.cols, c.frame.step);
                ptrsTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_calc3DByteDepthBackProject_ASM(c.frame.data, c.hist.data, c.ptrsBackproj.data, c.frame.rows, c.frame.cols, c.frame.step);
                asmTime += timer::t;
            }

            c.cvTime = cvTime / REPETITIONS;
            c.ptrsTime = ptrsTime / REPETITIONS;
            c.asmTime = asmTime / REPETITIONS;
        }

        cout << endl << "(área del frame, microsegundos)" << endl;
        cout << endl;
        cout << "Versión idiomática" << endl;
        // format print idiomatic
        for (backprojCase& c : backprojCases) {
            cout << "(" << c.frame.total() << ", " << c.cvTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión con punteros" << endl;
        // format print pointers
        for (backprojCase& c : backprojCases) {
            cout << "(" << c.frame.total() << ", " << c.ptrsTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión vectorial" << endl;
        // format print asm
        for (backprojCase& c : backprojCases) {
            cout << "(" << c.frame.total() << ", " << c.asmTime << ")" << endl;
        }

    } else if (string("meanshift") == argv[1]) {

        for (meanshiftCase& c : meanshiftCases) {
            double cvTime = 0;
            double ptrsTime = 0;
            double asmTime = 0;

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_meanShift_CV(c.densityMap, c.cvWindow, ITERS);
                cvTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_meanShift(c.densityMap.data, c.densityMap.rows, c.densityMap.cols, c.densityMap.step, &c.ptrsWindow, ITERS);
                ptrsTime += timer::t;
            }

            for (int i = 0; i < REPETITIONS; ++i) {
                IHT_meanShift_ASM(c.densityMap.data, c.densityMap.rows, c.densityMap.cols, c.densityMap.step, &c.asmWindow, ITERS);
                asmTime += timer::t;
            }

            c.cvTime = cvTime / REPETITIONS;
            c.ptrsTime = ptrsTime / REPETITIONS;
            c.asmTime = asmTime / REPETITIONS;
        }

        cout << endl << "(área de V, microsegundos)" << endl;
        cout << endl;
        cout << "Versión idiomática" << endl;
        // format print idiomatic
        for (meanshiftCase& c : meanshiftCases) {
            cout << "(" << c.cvWindow.total() << ", " << c.cvTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión con punteros" << endl;
        // format print pointers
        for (meanshiftCase& c : meanshiftCases) {
            cout << "(" << c.cvWindow.total() << ", " << c.ptrsTime << ")" << endl;
        }

        cout << endl;

        cout << "Versión vectorial" << endl;
        // format print asm
        for (meanshiftCase& c : meanshiftCases) {
            cout << "(" << c.cvWindow.total() << ", " << c.asmTime << ")" << endl;
        }

    }

    return 0;
}
