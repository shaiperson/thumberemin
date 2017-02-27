#include "../../include/game/Theremin.h"

// ./main or ./main vid <video_file_path>
int main(int argc, char* argv[]) {

    string option = argc > 1 ? argv[1] : "";

    Theremin theremin(option);
    theremin.runLive();

    return 0;
}
