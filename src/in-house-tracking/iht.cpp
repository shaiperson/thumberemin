/* In-house tracking  ============================================= */

#include "../../include/in-house-tracking/iht.h"

/* Supone hist continuas, hist inicializada en 0 */
void IHT_calc3DByteDepthUniformHist(const uchar* imgdata, uchar* histdata, size_t imgrows, size_t imgcols, size_t imgstep) {

    GLOBAL_startTimer();

    size_t chs = 3; // CHANNELS

    size_t dimSize = 256;
    size_t planeSize = 65536;

    for (size_t i = 0; i < imgrows; ++i) {
        for (size_t j = 0; j < imgcols; ++j) {
            const uchar* pixel = imgdata + i*imgstep + j*chs;
            uchar* bin =
                histdata +
                pixel[2] * planeSize * sizeof(ushort) +
                pixel[1] * dimSize * sizeof(ushort) +
                pixel[0] * sizeof(ushort);
            *(ushort*)bin += 1;
        }
    }

    GLOBAL_stopTimer();

}

void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist) {

    GLOBAL_startTimer();
    /*
    for (size_t i = 0; i < image.rows; ++i) {
        for (size_t j = 0; j < image.cols; ++j) {
            const Vec3b& pixel = image.at<Vec3b>(i,j);
            hist.at<ushort>(pixel[2], pixel[1], pixel[0]) += 1;
        }
    }
    */
    for (auto it = image.begin<Vec3b>(); it != image.end<Vec3b>(); ++it)
        hist.at<ushort>((*it)[2], (*it)[1], (*it)[0]) += 1;

    GLOBAL_stopTimer();

}

/* =================================================================
--------------------------------------------------------------------
================================================================= */

/* RESULT SE SUPONE INICIALIZADA EN 0 */
void IHT_calc3DByteDepthBackProject (
    const uchar* imgdata,
    const uchar* histdata,
    uchar* resdata,
    size_t imgrows,
    size_t imgcols,
    size_t imgstep
) {

    GLOBAL_startTimer();

    size_t imgchs = 3; // should be == image->channels()
    size_t reschs = 1;

    size_t padding = imgstep - imgcols * imgchs * sizeof(uchar);

    size_t dimSize = 256;
    size_t planeSize = dimSize * dimSize;

    size_t i = 0;
    size_t j;
    while (i < imgrows) {
        j = 0;
        while (j < imgcols) {
            * (ushort*) resdata = * (ushort*) (
                histdata +
                imgdata [2 * sizeof(uchar)] * planeSize * sizeof(ushort) +
                imgdata [1 * sizeof(uchar)] * dimSize * sizeof(ushort) +
                imgdata [0 * sizeof(uchar)] * sizeof(ushort)
            );
            imgdata += imgchs * sizeof(uchar);
            resdata += reschs * sizeof(ushort);
            j += 1;
        }
        imgdata += padding;
        i += 1;
    }

    GLOBAL_stopTimer();

}

void IHT_calc3DByteDepthBackProject_CV(const Mat& image, const Mat& hist, Mat& backProjection) {

    GLOBAL_startTimer();

    auto backprIt = backProjection.begin<ushort>();
    for (auto imageIt = image.begin<Vec3b>(); imageIt != image.end<Vec3b>(); ++imageIt)
        *backprIt++ = hist.at<ushort>((*imageIt)[2], (*imageIt)[1], (*imageIt)[0]);

    GLOBAL_stopTimer();

}

/* =================================================================
--------------------------------------------------------------------
================================================================= */

void IHT_meanShift (
    const uchar* densityMap,
    int maprows,
    int mapcols,
    int mapstep,
    IHT_window* w,
    size_t iters
) {

    GLOBAL_startTimer();

    ushort curr;
    float m00, m10, m01;
    int x, y;
    int shifted_centroid_x, shifted_centroid_y;
    int centroid_x, centroid_y;
    int tl_x, tl_y;
    const uchar *w_data, *w_column;

    int iterCounter = 0;
    int curr_w_x = w->x;
    int curr_w_y = w->y;
    int width = w->width;
    int height = w->height;

    while (iterCounter < iters) {

        m00 = m10 = m01 = 0;

        // calculate current window moments
        w_data = densityMap + curr_w_y*mapstep + curr_w_x*sizeof(ushort);

        //y = curr_w_y;
        y = 0; // do x,y positioning relative to window to avoid large moment values and consequent precision issues
        while (y < height) {
            //x = curr_w_x;
            x = 0;
            while (x < width) {
                // window is known to be in valid position within density map

                curr = *(ushort*)w_data;

                m00 += curr;
                m10 += x*curr;
                m01 += y*curr;

                x += 1;
                w_data += sizeof(ushort); // next element on row
            }

            y += 1;
            w_data += mapstep - width*sizeof(ushort); // next row beginning
        }

        /* update curr window */

        tl_x = roundf(m10/m00 + (float)curr_w_x - width*0.5);
        tl_y = roundf(m01/m00 + (float)curr_w_y - height*0.5);

        if ( // intersection between current and previous windows not empty
            tl_x >= 0 &&
            tl_y >= 0 &&
            abs(tl_x - curr_w_x) < width &&
            abs(tl_y - curr_w_y) < height
        ) {
            curr_w_x = std::min(tl_x, mapcols-width);
            curr_w_y = std::min(tl_y, maprows-height);
        }

        iterCounter += 1;
    }

    w->x = curr_w_x;
    w->y = curr_w_y;

    GLOBAL_stopTimer();
}

void IHT_meanShift_CV(const Mat& densityMap, IHT_window& window, size_t iters) {

    GLOBAL_startTimer();

    Point centroid, tl;
    Rect wRect;

    for (size_t iter = 0; iter < iters; ++iter) {
        wRect = Rect(window.tl(), window.size());

        iht_moments ms(densityMap(wRect));

        tl = Point (
            roundf(ms.m10/ms.m00 + (float)window.tl().x - window.width*0.5),
            roundf(ms.m01/ms.m00 + (float)window.tl().y - window.height*0.5)
        );

        if ( tl.x >= 0 && tl.y >= 0 && ( wRect & Rect(tl, window.size()) ) != Rect() ) {
            window = Rect (
                std::min(tl.x, densityMap.cols - window.width),
                std::min(tl.y, densityMap.rows - window.height),
                window.width,
                window.height
            );
        }
    }

    GLOBAL_stopTimer();
}

/* =================================================================
--------------------------------------------------------------------
================================================================= */

/* moments */

iht_moments::iht_moments(const Mat& data) : m00(0), m10(0), m01(0) {
    ushort curr;
    for (int x = 0; x < data.cols; ++x) {
        for (int y = 0; y < data.rows; ++y) {
            curr = data.at<ushort>(y,x);
            m00 += curr;
            m10 += x*curr;
            m01 += y*curr;
        }
    }
    centroid = Point(roundf(m10/m00), roundf(m01/m00));
}

/* aux */

Mat IHT_createHistArgumentShort() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_16UC1, Scalar(0));
}

Mat IHT_createBackProjectArgumentShort(const Size& size) {
    return Mat(size, CV_16UC1, Scalar(0));
}

void IHT_meanShift_CV(const Mat& densityMap, Rect& window, size_t iters);
