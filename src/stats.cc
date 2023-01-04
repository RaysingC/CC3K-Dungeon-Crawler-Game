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