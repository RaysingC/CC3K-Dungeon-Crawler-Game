#include "player.h"
#include "stats.h"
#include <iostream>

Player::~Player() {}

// plan: overload use() and trigger() for class abilities?
// also so much code duplication...
class HumanPlayer : public Player {
public:
    HumanPlayer(std::pair<int, int>&& startingPos)
        : Player('h', std::move(startingPos)) {}
};

class ElfPlayer : public Player {
public:
    ElfPlayer(std::pair<int, int>&& startingPos)
        : Player('e', std::move(startingPos)) {}
    // void use(Direction) override;
};

class DwarfPlayer : public Player {
public:
    DwarfPlayer(std::pair<int, int>&& startingPos)
        : Player('d', std::move(startingPos)) {}
    void pickup_gold(int amount) override {
        stats->change_gold(amount * 2);
    }
};

class OrcPlayer : public Player {
public:
    OrcPlayer(std::pair<int, int>&& startingPos)
        : Player('o', std::move(startingPos)) {}
    void pickup_gold(int amount) override {
        stats->change_gold((amount + 1) / 2);
    }
};

// inlines only work in the header file

Player::Player(char race, std::pair<int, int>&& startingPos)
    : stats{PlayerStats::init(race, std::move(startingPos))} {}

// race is checked to be valid by game.cc
std::unique_ptr<Player> Player::make_player(char race, std::pair<int, int>&& startingPos) noexcept {
    if (race == 'h') {
        return std::make_unique<HumanPlayer>(std::move(startingPos));
    } else if (race == 'e') {
        return std::make_unique<ElfPlayer>(std::move(startingPos));
    } else if (race == 'd') {
        return std::make_unique<DwarfPlayer>(std::move(startingPos));
    } else if (race == 'o') {
        return std::make_unique<OrcPlayer>(std::move(startingPos));
    } else {
        std::cerr << "Invalid race chosen, defaulting to Human\n";
        return std::make_unique<HumanPlayer>(std::move(startingPos));
    }
}