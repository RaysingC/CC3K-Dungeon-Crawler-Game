#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include "direction.h"
#include "stats.h"

// factor Player and Enemy classes out into a template or something
class Enemy {
protected:
    char type;
    std::unique_ptr<Stats> stats;
    Enemy(int, int, int, char, std::pair<int, int>&&);

public:
    virtual ~Enemy() = 0;
    static std::unique_ptr<Enemy> make_enemy(char, std::pair<int, int>&&);
    const Stats& get_stats() const noexcept { return *stats; }
};

#endif