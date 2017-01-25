#include "../include/global.h"
#include "../include/initialization.h"

int main() {

    Mat frame, frameCopy;
    VideoCapture cap = initializeVideoCapture();
    cap >> frame;

    namedWindow("camarita", 1);
    InitialScreen initialScreen(frame.size());

    while (true) {
        cap >> frameCopy;
        flip(frameCopy, frame, 1);

        initialScreen.updateFrame(frame);

        imshow("camarita", frame);

        int key = waitKey(1);
        if (key == 113) {
            cout << "K, quitting." << endl;
            break;
        } else if (key == 13) {
            cout << "Entering playing mode" << endl;
        }
    }

    return 0;
}
