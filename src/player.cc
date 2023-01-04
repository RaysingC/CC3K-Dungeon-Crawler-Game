#include "player.h"
#include "stats.h"
#include <iostream>

Player::~Player() {}

// plan: overload use() and trigger() for class abilities?
class HumanPlayer : public Player {
public:
    HumanPlayer(std::pair<int, int>&& pos)
        : Player(140, 20, 20, 'h', std::move(pos)) {}
};

class ElfPlayer : public Player {
public:
    ElfPlayer(std::pair<int, int>&& pos)
        : Player(140, 30, 10, 'e', std::move(pos)) {}
    // void use(Direction) override;
};

class DwarfPlayer : public Player {
public:
    DwarfPlayer(std::pair<int, int>&& pos)
        : Player(100, 20, 30, 'd', std::move(pos)) {}
    /*
    void pickup_gold(int amount) override {
        stats->change_gold(amount * 2);
    }
    */
};

class OrcPlayer : public Player {
public:
    OrcPlayer(std::pair<int, int>&& pos)
        : Player(180, 30, 25, 'o', std::move(pos)) {}
    /*
    void pickup_gold(int amount) override {
        stats->change_gold((amount + 1) / 2);
    }
    */
};

// inlines only work in the header file

Player::Player(int maxhp, int atk, int def, char race, std::pair<int, int>&& pos)
    : stats{std::make_unique<PlayerStats>(maxhp, atk, def, std::move(pos), race)} {}

// race is checked to be valid by game.cc
std::unique_ptr<Player> Player::make_player(char race, std::pair<int, int>&& pos) noexcept {
    if (race == 'h') {
        return std::make_unique<HumanPlayer>(std::move(pos));
    } else if (race == 'e') {
        return std::make_unique<ElfPlayer>(std::move(pos));
    } else if (race == 'd') {
        return std::make_unique<DwarfPlayer>(std::move(pos));
    } else if (race == 'o') {
        return std::make_unique<OrcPlayer>(std::move(pos));
    } else {
        std::cerr << "Invalid race chosen, defaulting to Human\n";
        return std::make_unique<HumanPlayer>(std::move(pos));
    }
}