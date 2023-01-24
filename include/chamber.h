#ifndef CHAMBER_H
#define CHAMBER_H

#include <string>
#include <array>
#include <memory>
#include <map>
#include "cell.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "chambersettings.h"

// class Player;
class PlayerStats;
enum class Direction;

class Chamber {
    using CellArray = std::array<Cell, ChamberSettings::width() * ChamberSettings::height()>;
    char race;
    CellArray grid;
    std::pair<char, Direction> playerNextAction;
    std::shared_ptr<Player> player;
    std::map<const std::pair<int, int>, std::unique_ptr<Item>> items;
    std::map<const std::pair<int, int>, std::unique_ptr<ContactItem>> citems;
    std::vector<std::unique_ptr<Enemy>> enemies;

    Cell& cell_in_dir(int, int, Direction) noexcept;

public:
    Chamber(const std::string& layout);
    void set_player_action(char, Direction);
    void set_race(char r) noexcept { race = r; }
    const std::shared_ptr<Player>& get_playerptr() const noexcept { return player; }

    void spawn_all(std::shared_ptr<Player>&& playerptr = nullptr);
    void next_turn();
    void print() const noexcept;
};

#endif