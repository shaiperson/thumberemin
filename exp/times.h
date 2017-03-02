#ifndef TIMES_H
#define TIMES_H

using namespace std;

struct SquareRandomSamplingCase {
    Mat window;
    Mat cvHist, ptrsHist, asmHist;
    SquareRandomSamplingCase(int rows, int cols) :
        window(rows, cols, CV_8UC3)
    {
        for (auto it = window.begin<Vec3b>(); it != window.end<Vec3b>(); ++it)
            *it = Vec3b(rand() % UCHAR_MAX, rand() % UCHAR_MAX, rand() % UCHAR_MAX);
        cvHist = ptrsHist = asmHist = IHT_createHistArgumentShort();
    }
};

struct SquareRandomBackprojCase {
    Mat image;
    Mat hist;
    Mat cvBackproj, ptrsBackproj, asmBackproj;
    SquareRandomBackprojCase(int rows, int cols) :
        image(rows, cols, CV_16UC1)
    {
        for (auto it = image.begin<Vec3b>(); it != image.end<Vec3b>(); ++it)
            *it = Vec3b(rand() % UCHAR_MAX, rand() % UCHAR_MAX, rand() % UCHAR_MAX);
        hist = IHT_createHistArgumentShort(); // all 0s; doesn't matter since only used to measure backproj time
        cvBackproj = ptrsBackproj = asmBackproj = IHT_createBackProjectArgumentShort(Size(cols, rows));
    }
};

struct SquareRandomMeanshiftCase {
    Mat densityMap;
    IHT_window cvWindow, ptrsWindow, asmWindow;

    SquareRandomMeanshiftCase(int rows, int cols, int wside) :
        densityMap(rows, cols, CV_16UC1),
        cvWindow (
            cols/2 - wside/2,
            rows/2 - wside/2,
            wside,
            wside
        ),
        ptrsWindow(cvWindow),
        asmWindow(cvWindow)
    {
        for (size_t i = 0; i < densityMap.rows; ++i)
            for (size_t j = 0; j < densityMap.cols; ++j)
                densityMap.at<ushort>(i,j) = rand() % USHRT_MAX;
    }
};

vector<SquareRandomSamplingCase> createSquareRandomSamplingCases (int sideMin, int sideMax, int sideStep) {
    vector<SquareRandomSamplingCase> result;
    for (int side = sideMin; side <= sideMax; side += sideStep)
        result.push_back(SquareRandomSamplingCase(side, side));
    return result;
}

vector<SquareRandomBackprojCase> createSquareRandomBackprojCases (int sideMin, int sideMax, int sideStep) {
    vector<SquareRandomBackprojCase> result;
    for (int side = sideMin; side <= sideMax; side += sideStep)
        result.push_back(SquareRandomBackprojCase(side, side));
    return result;
}

vector<SquareRandomMeanshiftCase> createSquareRandomMeanshiftCasesWithMidSquareWindow(int imgside, int wsideMin, int wsideMax, int wsideStep) {
    vector<SquareRandomMeanshiftCase> result;
    for (int wside = wsideMin; wside <= wsideMax; wside += wsideStep)
        result.push_back(SquareRandomMeanshiftCase(imgside, imgside, wside));
    return result;
}

#endif
