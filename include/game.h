#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>
#include "chamber.h"

// unique_ptrs need to know the size of the class to make default deleters ;-;
// class Chamber;

class Game {
    int floorNumber;
    std::unique_ptr<Chamber> chamberptr;

    void prompt_race();
    void print(const std::string&) const noexcept;
    void end_game_message() const;

public:
    Game(const std::string& layoutFile);
    void play();
};

#endif