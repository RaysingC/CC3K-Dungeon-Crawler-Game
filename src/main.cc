#include <iostream>
#include "game.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Error: too many arguments\n";
        return 1;
    }

    std::string chamberLayout{"./src/default_grid.txt"};
    if (argc == 2) {
        chamberLayout = argv[1];
    }

    Game g{chamberLayout};
    g.play();
}