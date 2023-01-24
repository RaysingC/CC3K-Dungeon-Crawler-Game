#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

enum class Direction { NW, N, NE, W, X, E, SW, S, SE, INVALID };

namespace DirUtils {
    Direction rand_dir();
    Direction str_input_to_dir(const std::string&) noexcept;
    bool valid_dir_input(const std::string&) noexcept;
    std::string dir_to_str(Direction) noexcept;
    
    // Bounds checking is applied here so players don't cause a segfault when moving at the edges
    std::pair<int, int> new_coords(const std::pair<int, int>&, Direction) noexcept;
}

#endif