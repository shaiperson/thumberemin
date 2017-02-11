#include "../../include/game/Theremin.h"

int main(int argc, char* argv[]) {

    VideoCapture cap(argv[1]);
    Mat frame;

    if(!cap.isOpened()) throw runtime_error("Error initializing video");

    // cap.set(CV_CAP_PROP_FPS, 15);

    while (cap.read(frame)){
        imshow("JAJA", frame);
        waitKey(20);
    }

    return 0;
}
