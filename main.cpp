#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <iostream>

using namespace cv;
using namespace std;

double darkenFactor = 0.3;

Mat getBrightnessKernel(double factor) {
    return (Mat_<double>(1,1) << factor);
}

double squaredDistance(const Point& p1, const Point& p2) {
    int diffx = p1.x - p2.x;
    int diffy = p1.y - p2.y;
    return diffx*diffx + diffy*diffy;
}

vector<Point2i> getCirclePoints(const Mat& image, const Point& center, int radius) {
    vector<Point2i> points;
    for (int x = center.x-radius; x <= center.x+radius; ++x)
        for (int y = center.y-radius; y <= center.y+radius; ++y)
            if (squaredDistance(Point(x,y), center) <= radius*radius)
                points.push_back(Point(x,y));
    // cout << points << endl;
    // return InputArray(points);
    return points;
}

Mat brightKernel = (Mat_<double>(1,1) << 2);
Mat reallyBrightKernel = (Mat_<double>(1,1) << 3);
Mat edgeDetectionKernel = (Mat_<double>(3,3) <<
    0, 1, 0, 1, -4, 1, 0, 1, 0);
Mat gaussianBlur = (Mat_<double>(3,3) <<
    1/16.0, 2/16.0, 1/16.0, 2/16.0, 4/16.0, 2/16.0, 1/16.0, 2/16.0, 1/16.0);

void scalePixels(Mat& m, const vector<Point2i>& ps, double factor) {
    for (auto it = ps.cbegin(); it != ps.cend(); ++it)
        m.at<Vec3b>(it->y, it->x) *= factor;
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame, frameCopy, left, right, stripe;

    namedWindow("camarita", 1);

    cap >> frame; // capture once to get dimension info
    double stripeLeftmostCol = (frame.cols)*(10/12.0);
    double stripeWidth = (frame.cols)/12.0;
    double stripeRightmostCol = stripeLeftmostCol + stripeWidth;

    Rect stripeRectangle(stripeLeftmostCol, 0, stripeWidth, frame.rows);
    Rect leftRectangle(0, 0, stripeLeftmostCol, frame.rows);
    Rect rightRectangle(stripeRightmostCol, 0, frame.cols-leftRectangle.width-stripeWidth-1, frame.rows);

    Point fingerSamplingCircleCenter(stripeRectangle.x + stripeRectangle.width/2, stripeRectangle.height*(10/12.0));
    int fingerSamplingCircleRadius = 25;

    Rect fingerSamplingRectangle(stripeRectangle.x+(1/3.0)*stripeWidth, stripeRectangle.height*(10/12.0), stripeWidth*(1/3.0), stripeWidth*(1/3.0));

    for(;;)
    {
        cap >> frameCopy;

        flip(frameCopy, frame, 1);

        /* el circulito es para el principio para samplear el dedo */
        // circle(frame, fingerSamplingCircleCenter, fingerSamplingCircleRadius, Scalar(0,0,255), -1);)

        // vector<Point2i> circlePoints = getCirclePoints(frame, fingerSamplingCircleCenter, fingerSamplingCircleRadius);
        // filter2D(frame, frame, -1, getBrightnessKernel(darkenFactor));
        // scalePixels(frame, circlePoints, 1.0/darkenFactor);

        /*===================================================*/

        /* el stripe es para la playing phase */

        left = frame(leftRectangle);
        right = frame(rightRectangle);
        stripe = frame(stripeRectangle);

        filter2D(left, left, -1, getBrightnessKernel(darkenFactor));
        filter2D(right, right, -1, getBrightnessKernel(darkenFactor));

        // circle(frame, fingerSamplingCircleCenter, fingerSamplingCircleRadius, Scalar(255,191,0), 4);
        rectangle(frame, fingerSamplingRectangle, Scalar(255,191,0), 4);

        imshow("camarita", frame);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
