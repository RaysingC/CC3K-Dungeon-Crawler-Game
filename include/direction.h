#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

// note: puting implementation directly in header files will cause the
//   functions to be compiled multiple times???
enum class Direction { NW, N, NE, W, X, E, SW, S, SE, INVALID };

namespace DirUtils {
    Direction rand_dir();

    Direction str_input_to_dir(const std::string&) noexcept;

    // I only use valid_dir to evaluate inputs so this one that
    //   returns true for Direction::X might not be needed
    // bool valid_dir(const std::string&);

    bool valid_dir_input(const std::string&) noexcept;
    std::string dir_to_str(Direction) noexcept;
    std::pair<int, int> new_coords(const std::pair<int, int>&, Direction) noexcept; // this is only used with the 'u' command
}

#endif