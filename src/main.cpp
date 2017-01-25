#include "../include/global.h"
#include "../include/Screen.h"
#include "../include/Theremin.h"

VideoCapture initializeVideoCapture() {
    VideoCapture cap(DEFAULT_CAMERA); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        throw runtime_error("Error initializing camera");
    return cap;
}

int main() {
    
    Theremin theremin;
    theremin.run();

    return 0;
}
