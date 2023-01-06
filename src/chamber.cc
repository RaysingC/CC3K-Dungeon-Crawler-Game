#include "chamber.h"
#include "player.h"
#include "stats.h"
#include "direction.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

// bounds checking is applied here so players don't cause a segfault when moving at the edges
Cell& Chamber::cell_in_dir(int x, int y, Direction dir) noexcept {
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
    return grid[y * ChamberSettings::width() + x];
}

using CellArray = std::array<Cell, ChamberSettings::width() * ChamberSettings::height()>;

CellArray make_grid(const std::string& layout) {
    CellArray cellarray;
    int size = layout.size();
    if (size != ChamberSettings::width() * ChamberSettings::height()) {
        std::cerr << "Error: Layout is not 79 by 25\n";
        throw "gridSizeError";
    }

    for (int i = 0; i < size; ++i)
        cellarray[i] = Cell{layout[i]};

    return cellarray;
}

Chamber::Chamber(const std::string& layout) : floorNumber{1}, race{'h'}, grid{make_grid(layout)},
    playerNextAction{std::make_pair('m', Direction::X)}, player{nullptr} {}

void Chamber::set_player_action(char action, Direction dir) /*noexcept*/ {
    auto& [ oldaction, olddir ] = playerNextAction;
    oldaction = action;
    olddir = dir;
}

const PlayerStats& Chamber::player_stats() const {
    return player->get_stats();
}

using vecOfCoords = std::vector<std::pair<int, int>>;

static void addCellsToChamber(CellArray& tempgrid, vecOfCoords& chamber, int x, int y) {
    Cell& cell = tempgrid[y * ChamberSettings::width() + x];
    if (cell.get_type() == '.') {
        chamber.emplace_back(std::make_pair(x, y));
        cell = Cell{'X'};
        for (int i = -1; i <= 1; ++i)
            for (int j = -1; j <= 1; ++j)
                addCellsToChamber(tempgrid, chamber, x + i, y + j);
    }
}

void Chamber::spawn_all() {
    // 1. number chambers
    std::vector<vecOfCoords> chambers;
    CellArray tempgrid = grid; // making a tempgrid is slow and heavy

    for (int y = 0; y < ChamberSettings::height(); ++y) {
        for (int x = 0; x < ChamberSettings::width(); ++x) {
            const Cell& cell = tempgrid[y * ChamberSettings::width() + x];
            if (cell.get_type() == '.') {
                chambers.emplace_back(vecOfCoords{});
                addCellsToChamber(tempgrid, chambers.back(), x, y);
            }
        }
    }

    // 2. spawn player in one of them
    std::shuffle(chambers.begin(), chambers.end(), ChamberSettings::get_generator());

    for (auto& chamber : chambers) {
        std::shuffle(chamber.begin(), chamber.end(), ChamberSettings::get_generator());
    }

    player = Player::make_player(race, std::move(chambers[0].back()));
    chambers[0].pop_back();
}

void Chamber::next_turn() /*noexcept*/ {
    auto [ action, dir ] = playerNextAction;
    const auto& [ playerx, playery ] = player->get_stats().get_pos();
    Cell& currentCell = grid[playery * ChamberSettings::width() + playerx];
    Cell& targetCell = cell_in_dir(playerx, playery, dir);

    if (action == 'm') {
        currentCell.unoccupy();
        if (targetCell.is_occupied()) {
            dir = Direction::X;
        }
        player->move(dir);
        targetCell.occupy();
    } else if (action == 'u') {
        
    } else if (action == 'a') {

    }
    // call passives on player and enemies, check descend logic?
}

void Chamber::print() const noexcept {
    constexpr int totalSize = ChamberSettings::width() * ChamberSettings::height();
    std::array<char, totalSize> display;
    for (int i = 0; i < totalSize; ++i) {
        display[i] = grid[i].get_type();
    }

    const auto& [ playerx, playery ] = player->get_stats().get_pos();
    display[playery * ChamberSettings::width() + playerx] = '@';

    // do the same for enemies

    for (int y = 0; y < ChamberSettings::height(); ++y) {
        for (int x = 0; x < ChamberSettings::width(); ++x)
            std::cout << display[y * ChamberSettings::width() + x];
        std::cout << '\n';
    }
}