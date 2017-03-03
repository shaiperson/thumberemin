#ifndef TIMES_H
#define TIMES_H

using namespace std;

struct samplingCase {
    Mat window;
    Mat cvHist, ptrsHist, asmHist;
    double cvTime, ptrsTime, asmTime;

    samplingCase(int rows, int cols) :
        window(rows, cols, CV_8UC3)
    {
        uchar x = 0;
        for (auto it = window.begin<Vec3b>(); it != window.end<Vec3b>(); ++it)
            *it = Vec3b(x++, x++, x++);
        cvHist = ptrsHist = asmHist = IHT_createHistArgumentShort();
    }
};

struct backprojCase {
    Mat frame;
    Mat hist;
    Mat cvBackproj, ptrsBackproj, asmBackproj;
    double cvTime, ptrsTime, asmTime;

    backprojCase(int rows, int cols) :
        frame(rows, cols, CV_8UC3)
    {
        uchar x = 0;
        for (auto it = frame.begin<Vec3b>(); it != frame.end<Vec3b>(); ++it)
            *it = Vec3b(x++, x++, x++);
        hist = IHT_createHistArgumentShort(); // all 0s; doesn't matter since only used to measure backproj time
        cvBackproj = ptrsBackproj = asmBackproj = IHT_createBackProjectArgumentShort(frame.size());
    }
};

struct meanshiftCase {
    Mat densityMap;
    IHT_window cvWindow, ptrsWindow, asmWindow;
    double cvTime, ptrsTime, asmTime;

    meanshiftCase(int rows, int cols, int wside) :
        densityMap(rows, cols, CV_16UC1),
        cvWindow (0, 0, wside, wside),
        ptrsWindow(cvWindow),
        asmWindow(cvWindow)
    {
        for (size_t i = 0; i < densityMap.rows; ++i)
            for (size_t j = 0; j < densityMap.cols; ++j)
                densityMap.at<ushort>(i,j) = 0;
    }
};

vector<samplingCase> createsamplingCases (int sideMin, int sideMax, int sideStep) {
    vector<samplingCase> result;
    for (int side = sideMin; side < sideMax; side += sideStep)
        result.push_back(samplingCase(side, side));
    return result;
}

vector<backprojCase> createbackprojCases (int sideMin, int sideMax, int sideStep) {
    vector<backprojCase> result;
    for (int side = sideMin; side < sideMax; side += sideStep)
        result.push_back(backprojCase(side, side));
    return result;
}

vector<meanshiftCase> createmeanshiftCasesWithMidSquareWindow(int imgside, int wsideMin, int wsideMax, int wsideStep) {
    vector<meanshiftCase> result;
    for (int wside = wsideMin; wside < wsideMax; wside += wsideStep)
        result.push_back(meanshiftCase(imgside, imgside, wside));
    return result;
}

#endif
