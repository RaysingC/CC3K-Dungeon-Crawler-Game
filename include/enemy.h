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
    bool playerInRangeLastTurn, playerInRangeNow, hostile;
    Enemy(int, int, int, char, std::pair<int, int>&&, bool);

public:
    virtual ~Enemy() = 0;
    static std::unique_ptr<Enemy> make_enemy(std::pair<int, int>&&);
    std::pair<int, int> get_pos() const noexcept { return stats.get_pos(); }
    char get_icon() const noexcept { return type; }

    // called per turn
    virtual void move(Direction dir) noexcept { stats.move(dir); }
    virtual void passive() noexcept {}
    bool player_in_range(const std::shared_ptr<Player>&) noexcept;

    // combat logic
    virtual void tank(int) noexcept;
    bool is_alive() const noexcept;
    virtual bool is_hostile() const noexcept { return hostile; }
    virtual std::unique_ptr<ContactItem> drop_item() const {
        return nullptr;
    }
    virtual void attempt_attack(const std::shared_ptr<Player>&) noexcept; // called every turn, attacks when conditions are met
};

class Merchant : public Enemy {
    static inline bool hostile{false}; // usually we initialize it in enemy.cc

public:
    Merchant(std::pair<int, int>&& pos) : Enemy(30, 70, 5, 'M', std::move(pos), false) {}
    std::unique_ptr<ContactItem> drop_item() const override;
    void tank(int) noexcept override;
    void attempt_attack(const std::shared_ptr<Player>&) noexcept override;
    bool is_hostile() const noexcept override { return Merchant::hostile; }

    static void pacify_merchants() noexcept { Merchant::hostile = false; } // called at the beginning of the game
};

#endif