#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "direction.h"
#include "stats.h"

class Player {
protected:
    std::unique_ptr<PlayerStats> stats;
    Player(char, std::pair<int, int>&&);
    // virtual void trigger(); This will be for gold pickups

public:
    virtual ~Player() = 0;
    static std::unique_ptr<Player> make_player(char, std::pair<int, int>&&) noexcept;
    const PlayerStats& get_stats() const noexcept { return *stats; }

    void move(Direction dir) noexcept { stats->move(dir); }
    // virtual void use(Direction);
    // void attack(Direction);
    virtual void pickup_gold(int amount) { stats->change_gold(amount); }
};

#endif