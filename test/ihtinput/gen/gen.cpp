#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <cstdlib>

using namespace cv;
using namespace std;

#define TYPE CV_8UC3

Mat alternate(int rows, int cols, const vector<Vec3b>& colors) {
    Mat result(rows, cols, TYPE);
    size_t k = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.at<Vec3b>(i,j) = colors[k++];
            k %= colors.size();
        }
    }
    return result;
}

Mat random(int rows, int cols) {
    Mat result(rows, cols, TYPE);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result.at<Vec3b>(i,j) = Vec3b(rand() % 256, rand() % 256, rand() % 256);
        }
    }
    return result;
}

/*
    ./main onecolor <rows> <cols> <B> <G> <R>
    ./main alternate <rows> <cols> <ncolors>  <-- stdin n colors
    ./main random <rows> <cols>'    '
*/
int main(int argc, char* argv[]) {

    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);

    Mat result;

    if (!strcmp(argv[1], "onecolor")) {

        result = Mat(rows, cols, TYPE, Scalar(atoi(argv[4]), atoi(argv[5]), atoi(argv[6])));

    } else if (!strcmp(argv[1], "alternate")) {

        vector<Vec3b> colors;
        int b,g,r;
        for (size_t i = 0; i < atoi(argv[4]); ++i) {
            cin >> b >> g >> r;
            colors.push_back(Vec3b(b,g,r));
        }
        result = alternate(rows, cols, colors);

    } else if (!strcmp(argv[1], "random")) {
        cout << "jajaj" << endl;
        cout << rows << " " << cols << endl;
        result = random(rows, cols);

    }

    imwrite("output.png", result);

    return 0;
}
