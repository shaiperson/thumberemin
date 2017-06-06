#include "../../include/game/Theremin.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Usage: ./main <MIDI channel>" << endl;
        exit(EXIT_FAILURE);
    }

    uchar midiChannel = (uchar)atoi(argv[1]);

    Theremin theremin(midiChannel);
    theremin.run();

    return 0;
}
