#include "../../include/game/Game.h"

int main(int argc, char* argv[]) {

    string option = argc > 1 ? argv[1] : "";

    Game game(option);
    game.runLive();

    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "El tracking corrió en un promedio de ";
    cout << timer::sampling + timer::trackingAccum / timer::trackingRepetitions;
    cout << " microsegundos." << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;

    return 0;
}
