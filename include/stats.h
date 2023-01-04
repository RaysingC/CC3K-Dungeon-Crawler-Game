#ifndef STATS_H
#define STATS_H

#include <tuple>
#include <memory>
#include "direction.h"

// is it okay for these classes to not be polymorphic?
class Stats {
protected:
    int maxhp, hp, atk, def;
    std::pair<int, int> pos;

public:
    Stats(int maxhp, int atk, int def, std::pair<int, int>&& pos)
        : maxhp{maxhp}, hp{maxhp}, atk{atk}, def{def}, pos{std::move(pos)} {}
    std::tuple<int, int, int> get_tuple() const {
        return std::make_tuple(hp, atk, def);
    }
    const std::pair<int, int>& get_pos() const noexcept { return pos; }
    void move(Direction) noexcept;
};

class PlayerStats : public Stats {
    char race;
    int gold;

public:
    PlayerStats(int maxhp, int atk, int def, std::pair<int, int>&& pos, char race)
        : Stats(maxhp, atk, def, std::move(pos)), race{race}, gold{0} {}
    std::tuple<int, int, int, char, int> get_tuple() const noexcept {
        return std::make_tuple(hp, atk, def, race, gold);
    }
    void change_gold(int change) {
        if (gold + change > 0) {
            gold += change;
        } else {
            gold = 0;
        }
    }
};

#endif