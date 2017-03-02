#ifndef TIMES_H
#define TIMES_H

using namespace std;

struct randomMeanshiftCase {
    Mat image;
    Rect ihtCvWindow, cvWindow;
    window ihtAsmWindow, ihtPtrsWindow;
    randomMeanshiftCase() :
        image(100, 100, CV_16UC1),
        ihtCvWindow(
            rand() % image.cols/4,
            rand() % image.rows/4,
            rand() % (image.cols*3/4),
            rand() % (image.rows*3/4)
        ),
        cvWindow(ihtCvWindow)
    {
        ihtAsmWindow = {ihtCvWindow.x, ihtCvWindow.y, ihtCvWindow.width, ihtCvWindow.height};
        ihtPtrsWindow = {ihtCvWindow.x, ihtCvWindow.y, ihtCvWindow.width, ihtCvWindow.height};
        for (size_t i = 0; i < image.rows; ++i) {
            for (size_t j = 0; j < image.cols; ++j) {
                image.at<short>(i,j) = rand() % SHRT_MAX;
            }
        }
    }
};

#endif
