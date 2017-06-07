#include "../../include/game/Theremin.h"

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Specify window size: ./main <width> <height>" << endl;
        exit(EXIT_FAILURE);
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    Theremin theremin(width, height);
    theremin.run();

    return 0;
}
