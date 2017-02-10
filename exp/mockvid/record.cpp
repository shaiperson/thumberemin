#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(){

    VideoCapture vcap(0);
      if(!vcap.isOpened()){
         cout << "Error initializaing le camera" << endl;
         return -1;
      }

   int frame_width = vcap.get(CV_CAP_PROP_FRAME_WIDTH);
   int frame_height = vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
   VideoWriter video("out", CV_FOURCC('I', 'Y', 'U', 'V'), 60.01, Size(frame_width,frame_height), true);

   while (1) {
        Mat frame;
        vcap >> frame;
        video.write(frame);
   }

  return 0;
}
