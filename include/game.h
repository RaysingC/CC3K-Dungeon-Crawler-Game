#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>
#include "chamber.h"

class Game {
    Chamber chamber;

    void prompt_race();
    void print(const std::string&) const noexcept;
    void end_game_message() const noexcept;

public:
    Game(const std::string& layoutFile);
    void play();
};

#endif