#include "direction.h"
#include <random>
#include <chrono>

namespace DirUtils {
    Direction rand_dir() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng{seed};
        return static_cast<Direction>(rng() % 9);
    }

    Direction str_input_to_dir(const std::string& dir) noexcept {
        if (dir == "nw") {
            return Direction::NW;
        } else if (dir == "n") {
            return Direction::N;
        } else if (dir == "ne") {
            return Direction::NE;
        } else if (dir == "w") {
            return Direction::W;
        } else if (dir == "x") {
            return Direction::X;
        } else if (dir == "e") {
            return Direction::E;
        } else if (dir == "sw") {
            return Direction::SW;
        } else if (dir == "s") {
            return Direction::S;
        } else if (dir == "se") {
            return Direction::SE;
        } else {
            return Direction::INVALID;
        }
    }

    /*
    bool valid_dir(const std::string& dir) {
        return str_input_to_dir(dir) != Direction::INVALID;
    }
    */

    bool valid_dir_input(const std::string& dir) noexcept {
        Direction d = str_input_to_dir(dir);
        return d != Direction::INVALID && d != Direction::X;
    }

    std::string dir_to_str(Direction dir) noexcept {
        switch(dir) {
            case Direction::NW:
                return "NW";
            case Direction::N:
                return "N";
            case Direction::NE:
                return "NE";
            case Direction::W:
                return "W";
            case Direction::X:
                return "X";
            case Direction::E:
                return "E";
            case Direction::SW:
                return "SW";
            case Direction::S:
                return "S";
            case Direction::SE:
                return "SE";
            default:
                return "INVALID";
        }
    }
}