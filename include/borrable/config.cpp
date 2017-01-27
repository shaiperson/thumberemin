#include "config.h"

int main() {
    cout << config::juancho << endl;
    cout << config::hola << endl;
    config::hola = "loool";
    cout << config::hola << endl;
    return 0;
}
