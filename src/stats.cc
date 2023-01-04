#include "stats.h"

// looks like its duplicating cell_in_dir in chamber.cc
void Stats::move(Direction dir) noexcept {
    auto& [ x, y ] = pos;
    switch(dir) {
        case Direction::NW:
            --x; --y;
            break;
        case Direction::N:
            --y;
            break;
        case Direction::NE:
            ++x; --y;
            break;
        case Direction::W:
            --x;
            break;
        case Direction::E:
            ++x;
            break;
        case Direction::SW:
            --x; ++y;
            break;
        case Direction::S:
            ++y;
            break;
        case Direction::SE:
            ++x; ++y;
            break;
        default:
            break;
    }
}

// change this implementation to work with classes in the future
std::unique_ptr<PlayerStats> PlayerStats::init(char race, std::pair<int, int>&& pos) {
    int maxhp, atk, def;
    if (race == 'h') {
        maxhp = 140;
        atk = 20;
        def = 20;
    } else if (race == 'e') {
        maxhp = 140;
        atk = 30;
        def = 10;
    } else if (race == 'd') {
        maxhp = 100;
        atk = 20;
        def = 30;
    } else if (race == 'o') {
        maxhp = 180;
        atk = 30;
        def = 25;
    }
    // I will allow this because only one heap allocation is happening so
    // I don't need the exception safety from make_unique
    return std::unique_ptr<PlayerStats>(new PlayerStats{maxhp, atk, def, std::move(pos), race});
}