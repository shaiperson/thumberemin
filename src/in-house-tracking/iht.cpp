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
                pixel[2] * planeSize * sizeof(short) +
                pixel[1] * dimSize * sizeof(short) +
                pixel[0] * sizeof(short);
            *(short*)bin += 1;
        }
    }

    GLOBAL_stopTimer();

}

void IHT_calc3DByteDepthUniformHist_CV(const Mat& image, Mat& hist) {

    GLOBAL_startTimer();

    for (size_t i = 0; i < image.rows; ++i) {
        for (size_t j = 0; j < image.cols; ++j) {
            const Vec3b& pixel = image.at<Vec3b>(i,j);
            hist.at<short>(pixel[2], pixel[1], pixel[0]) += 1;
        }
    }

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
            * (short*) resdata = * (short*) (
                histdata +
                imgdata [2 * sizeof(uchar)] * planeSize * sizeof(short) +
                imgdata [1 * sizeof(uchar)] * dimSize * sizeof(short) +
                imgdata [0 * sizeof(uchar)] * sizeof(short)
            );
            imgdata += imgchs * sizeof(uchar);
            resdata += reschs * sizeof(short);
            j += 1;
        }
        imgdata += padding;
        i += 1;
    }

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
    window* w,
    size_t iters
) {

    GLOBAL_startTimer();

    short curr;
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

    const uchar* window_ptr = densityMap + curr_w_y*mapstep + curr_w_x*sizeof(short);

    while (iterCounter < iters) {

        m00 = m10 = m01 = 0;

        // calculate current window moments
        w_data = window_ptr;

        //y = curr_w_y;
        y = 0; // do x,y positioning relative to window to avoid large moment values and consequent precision issues
        while (y < height) {
            //x = curr_w_x;
            x = 0;
            while (x < width) {
                // window is known to be in valid position within density map
                curr = *(short*)w_data;

                m00 += curr;
                m10 += x*curr;
                m01 += y*curr;

                x += 1;
                w_data += sizeof(short); // next element on row
            }

            y += 1;
            w_data += mapstep - width*sizeof(short); // next row beginning
        }

        /* update curr window */

        shifted_centroid_x = round(m10/m00);
        shifted_centroid_y = round(m01/m00);

        centroid_x = shifted_centroid_x + curr_w_x;
        centroid_y = shifted_centroid_y + curr_w_y;

        tl_x = centroid_x - width/2;
        tl_y = centroid_y - height/2;

        /* HACER EL MIN MAX IN-HOUSE */
        curr_w_x = std::min(std::max(tl_x, 0), mapcols-width);
        curr_w_y = std::min(std::max(tl_y, 0), maprows-height);

        iterCounter += 1;
    }

    w->x = curr_w_x;
    w->y = curr_w_y;

    GLOBAL_stopTimer();
}

void IHT_meanShift_CV(const Mat& densityMap, Rect& window, size_t iters) {

    GLOBAL_startTimer();

    Point centroid, shifted_tl;

    for (size_t iter = 0; iter < iters; ++iter) {

        iht_moments ms(densityMap(window));
        centroid = ms.centroid + window.tl();

        shifted_tl = centroid - Point(window.width/2, window.height/2);

        window = Rect (
            std::min(std::max(shifted_tl.x, 0), densityMap.cols - window.width),
            std::min(std::max(shifted_tl.y, 0), densityMap.rows - window.height),
            window.width,
            window.height
        );
    }

    GLOBAL_stopTimer();
}

/* =================================================================
--------------------------------------------------------------------
================================================================= */

/* moments */

iht_moments::iht_moments(const Mat& data) : m00(0), m10(0), m01(0) {
    short curr;
    for (int x = 0; x < data.cols; ++x) {
        for (int y = 0; y < data.rows; ++y) {
            curr = data.at<short>(y,x);
            m00 += curr;
            m10 += x*curr;
            m01 += y*curr;
        }
    }
    centroid = Point(round(m10/m00), round(m01/m00));
}

/* aux */

/* Consuming method requires 0-initialized */
Mat IHT_createHistArgumentFloat() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_32FC1, Scalar(0));
}

Mat IHT_createHistArgumentShort() {
    int histSize[] = {256, 256, 256}; // each color is in uchar range 0..255
    return Mat(3, histSize, CV_16UC1, Scalar(0));
}

/* Consuming method requires 0-initialized */
Mat IHT_createBackProjectArgumentFloat(const Size& size) {
    return Mat(size, CV_32FC1, Scalar(0));
}

Mat IHT_createBackProjectArgumentShort(const Size& size) {
    return Mat(size, CV_16UC1, Scalar(0));
}

void IHT_meanShift_CV(const Mat& densityMap, Rect& window, size_t iters);
