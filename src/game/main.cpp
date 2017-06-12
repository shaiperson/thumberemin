#include "../../include/game/Theremin.h"

int main(int argc, char* argv[]) {

    // default MIDI range values: lowest note C3 and three octaves in total
    uchar midiLow = 60;
    int totalNotes = 3*12;

    if (argc < 3) {
        cout << "Specify window size: ./main <width> <height>" << endl;
        cout << "Optionally specify MIDI range: ./main <w> <h> <MIDI lowest> <total notes>" << endl;
        exit(EXIT_FAILURE);
    } else if (argc == 5) {
        midiLow = (uchar)atoi(argv[3]);
        totalNotes = atoi(argv[4]);
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    Theremin theremin(width, height, midiLow, totalNotes);
    theremin.run();

    return 0;
}
