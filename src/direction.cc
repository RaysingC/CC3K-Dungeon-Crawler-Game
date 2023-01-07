#include "direction.h"
#include <random>
#include <chrono>
#include "chambersettings.h"

namespace DirUtils {
    Direction rand_dir() {
        std::default_random_engine& rng{ChamberSettings::get_generator()};
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

    std::pair<int, int> new_coords(const std::pair<int, int>& coords, Direction dir) noexcept {
        auto [ x, y ] = coords;
        bool onLeftEdge = x == 0,
            onRightEdge = x == ChamberSettings::width() - 1,
            onTopEdge = y == 0,
            onBottomEdge = y == ChamberSettings::height() - 1;
        switch(dir) {
            case Direction::NW:
                if (!onTopEdge && !onLeftEdge) {
                    --x; --y;
                }
                break;
            case Direction::N:
                if (!onTopEdge)
                    --y;
                break;
            case Direction::NE:
                if (!onTopEdge && !onRightEdge) {
                    ++x; --y;
                }
                break;
            case Direction::W:
                if (!onLeftEdge)
                    --x;
                break;
            case Direction::E:
                if (!onRightEdge)
                    ++x;
                break;
            case Direction::SW:
                if (!onBottomEdge && !onLeftEdge) {
                    --x; ++y;
                }
                break;
            case Direction::S:
                if (!onBottomEdge)
                    ++y;
                break;
            case Direction::SE:
                if (!onBottomEdge && !onRightEdge) {
                    ++x; ++y;
                }
                break;
            default:
                break;
        }
        return std::make_pair(x, y);
    }
}