#include "player.h"
#include "stats.h"
#include <iostream>
#include <cmath>

Player::~Player() {}

class ConcretePlayer : public Player {
public:
    using Player::Player;
    std::shared_ptr<Player> remove_effects() noexcept override {
        return std::shared_ptr<ConcretePlayer>(new ConcretePlayer{*this});
    }
};

// plan: overload use() and trigger() for class abilities?
class HumanPlayer : public ConcretePlayer {
public:
    HumanPlayer(std::pair<int, int>&& pos)
        : ConcretePlayer(140, 20, 20, 'h', std::move(pos)) {}
};

class ElfPlayer : public ConcretePlayer {
public:
    ElfPlayer(std::pair<int, int>&& pos)
        : ConcretePlayer(140, 30, 10, 'e', std::move(pos)) {}
    // void use(Direction) override;
};

class DwarfPlayer : public ConcretePlayer {
public:
    DwarfPlayer(std::pair<int, int>&& pos)
        : ConcretePlayer(100, 20, 30, 'd', std::move(pos)) {}
    void change_gold(int amount) override {
        amount >= 0 ? gold += amount * 2 : gold += amount;
    }
};

class OrcPlayer : public ConcretePlayer {
public:
    OrcPlayer(std::pair<int, int>&& pos)
        : ConcretePlayer(180, 30, 25, 'o', std::move(pos)) {}
    void change_gold(int amount) override {
        amount >= 0 ? gold += (amount + 1) / 2 : gold += amount; // round up
    }
};

// inlines only work in the header file

Player::Player(int maxhp, int atk, int def, char race, std::pair<int, int>&& pos)
    : stats{maxhp, atk, def, std::move(pos)}, race{race}, gold{0} {}

// race is checked to be valid by game.cc
std::shared_ptr<Player> Player::make_player(char race, std::pair<int, int>&& pos) noexcept {
    if (race == 'h') {
        return std::make_shared<HumanPlayer>(std::move(pos));
    } else if (race == 'e') {
        return std::make_shared<ElfPlayer>(std::move(pos));
    } else if (race == 'd') {
        return std::make_shared<DwarfPlayer>(std::move(pos));
    } else if (race == 'o') {
        return std::make_shared<OrcPlayer>(std::move(pos));
    } else {
        std::cerr << "Invalid race chosen, defaulting to Human\n";
        return std::make_shared<HumanPlayer>(std::move(pos));
    }
}

std::tuple<int, int, int, char, int> Player::get_stats() const noexcept {
    auto [ hp, atk, def] = stats.get_tuple();
    return std::make_tuple(hp, atk, def, race, gold);
}

void Player::tank(int enemyAttack) noexcept {
    auto [ hp, atk, def] = stats.get_tuple();
    int totalDamage = ceil(100.0 / (100.0 + static_cast<double>(def) * static_cast<double>(enemyAttack)));
    stats.change_hp(-totalDamage);
}

void PlayerDecorator::tank(int enemyAttack) noexcept {
    auto [ hp, atk, def] = stats.get_tuple();
    int totalDamage = ceil(100.0 / (100.0 + static_cast<double>(def) * static_cast<double>(enemyAttack)));
    stats.change_hp(-totalDamage);
    playerptr->change_hp(-totalDamage);
}

// you can see how decorators of Players don't actually need their own stats...
void BarrierSuitPlayer::tank(int enemyAttack) noexcept {
    auto [ hp, atk, def, race, gold ] = get_stats();
    int totalDamage = ceil(100.0 / (100.0 + static_cast<double>(def) * static_cast<double>(enemyAttack)));
    stats.change_hp(-(totalDamage + 1) / 2);
}