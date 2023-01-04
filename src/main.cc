#include <iostream>
#include "game.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Error: too many arguments";
        return 1;
    }

    std::string chamberLayout{"default.txt"};
    if (argc == 2) {
        chamberLayout = argv[1];
    }

    Game g{chamberLayout};
    g.play();
}