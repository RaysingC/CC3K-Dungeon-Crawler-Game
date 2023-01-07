#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include "direction.h"
#include "stats.h"
#include "item.h"

// factor Player and Enemy classes out into a base class or something
class Enemy {
protected:
    Stats stats;
    char type;
    bool playerInRangeLastTurn, playerInRangeNow;
    Enemy(int, int, int, char, std::pair<int, int>&&);

public:
    virtual ~Enemy() = 0;
    static std::unique_ptr<Enemy> make_enemy(char, std::pair<int, int>&&);
    std::pair<int, int> get_pos() const noexcept { return stats.get_pos(); }
    char get_type() const noexcept { return type; }

    // called per turn
    virtual void move(Direction dir) noexcept { stats.move(dir); }
    virtual void passive() noexcept {}
    void check_player_in_range(const std::unique_ptr<Player>&) noexcept;

    // combat logic
    void tank(int) noexcept;
    bool is_alive() const noexcept {
        const auto& [ hp, atk, def ] = stats.get_tuple();
        if (hp <= 0) return true;
        return false;
    }
    virtual std::unique_ptr<ContactItem> drop_item() const {
        return nullptr;
    }
    void attempt_attack(const std::unique_ptr<Player>&) /*const*/ noexcept;
};

#endif