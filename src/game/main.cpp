#include "../../include/game/Theremin.h"

// ./main or ./main vid <video_file_path>
int main(int argc, char* argv[]) {

    if (argc > 1 && !(strcmp(argv[1], "vid"))) {
        Theremin theremin(argv[2]);
        theremin.runFromVideoSource();
    } else {
        Theremin theremin;
        theremin.runLive();
    }



    return 0;
}
