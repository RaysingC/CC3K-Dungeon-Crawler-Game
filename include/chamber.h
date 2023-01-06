#ifndef CHAMBER_H
#define CHAMBER_H

#include <string>
#include <array>
#include <memory>
#include "cell.h"
#include "player.h"
#include "chambersettings.h"

// class Player;
class PlayerStats;
enum class Direction;

class Chamber {
    using CellArray = std::array<Cell, ChamberSettings::width() * ChamberSettings::height()>;

    int floorNumber;
    char race;
    CellArray grid;
    std::pair<char, Direction> playerNextAction; // consider using a scoped enum for actions instead of a char
    std::unique_ptr<Player> player;
    // std::vector<std::unique_ptr<Enemy>> enemies;
    // std::vector<std::unique_ptr<Item>> items;

    Cell& cell_in_dir(int, int, Direction) noexcept; // invalid or out of bounds returns the current cell

public:
    Chamber(const std::string& layout); // constructs the map with no player
    void set_player_action(char, Direction); // do nothing if player == nullptr
    void set_race(char r) noexcept { race = r; }
    const PlayerStats& player_stats() const;

    void spawn_all();
    void next_turn();
    void print() const noexcept; // should this have noexcept?
    int get_floor() const noexcept { return floorNumber; }
};

#endif