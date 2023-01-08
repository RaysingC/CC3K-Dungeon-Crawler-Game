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
    std::pair<int, int> get_pos() const noexcept { return pos; }
    std::tuple<int, int, int> get_tuple() const noexcept {
        return std::make_tuple(hp, atk, def);
    }
    void move(Direction dir) noexcept {
        pos = DirUtils::new_coords(pos, dir);
    }

    // see player.h
    void change_hp(int change) noexcept { hp + change > 0 ? hp += change : hp = 0; }
    void change_atk(int change) noexcept { atk + change > 0 ? atk += change : atk = 0; }
    void change_def(int change) noexcept { def + change > 0 ? def += change : def = 0; }
    void set_pos(std::pair<int, int>&& pos) noexcept { pos = std::move(pos); }
};

#endif