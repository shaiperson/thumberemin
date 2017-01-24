#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame, frameCopy, left, right, stripe;

    namedWindow("camarita", 1);

    // capture once to get dimension info
    cap >> frame;

    double stripeLeftmostCol = (frame.cols)*(10/12.0);
    double stripeWidth = (frame.cols)/12.0;
    double stripeRightmostCol = stripeLeftmostCol + stripeWidth;

    Rect stripeRectangle(stripeLeftmostCol, 0, stripeWidth, frame.rows);
    Rect leftRectangle(0, 0, stripeLeftmostCol, frame.rows);
    Rect rightRectangle(stripeRightmostCol, 0, frame.cols-leftRectangle.width-stripeWidth-1, frame.rows);

    Rect fingerSamplingRectangle(stripeRectangle.x+(1/3.0)*stripeWidth, stripeRectangle.height*(10/12.0), stripeWidth*(1/3.0), stripeWidth*(1/3.0));

    for(;;)
    {
        cap >> frameCopy;

        flip(frameCopy, frame, 1);

        left = frame(leftRectangle);
        right = frame(rightRectangle);
        stripe = frame(stripeRectangle);

        // darken left and right sections
        left.convertTo(left, -1, 0.3, 0);
        right.convertTo(right, -1, 0.3, 0);

        // circle(frame, fingerSamplingCircleCenter, fingerSamplingCircleRadius, Scalar(255,191,0), 4);
        rectangle(frame, fingerSamplingRectangle, Scalar(255,191,0), 4);

        imshow("camarita", frame);

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
