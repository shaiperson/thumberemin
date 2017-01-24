#include "../include/global.h"
#include "../include/initialization.h"

int main() {

    Mat frame, frameCopy;
    VideoCapture cap = initializeVideoCapture();
    cap >> frame;
    InitialScreen initialScreen(frame.size());

    while (true) {
        cap >> frameCopy;
        flip(frameCopy, frame, 1);

        initialScreen.updateFrame(frame);

        imshow("camarita", frame);
        // return 0;
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
