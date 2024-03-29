#include "chamber.h"
#include "player.h"
#include "stats.h"
#include "direction.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "enemy.h"

Cell& Chamber::cell_in_dir(int x, int y, Direction dir) noexcept {
    auto [ newx, newy ] = DirUtils::new_coords(std::make_pair(x, y), dir);
    return grid[newy * ChamberSettings::width() + newx];
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

Chamber::Chamber(const std::string& layout) : race{'h'}, grid{make_grid(layout)},
    playerNextAction{std::make_pair('m', Direction::X)}, player{nullptr}, items{}, citems{} {}

void Chamber::set_player_action(char action, Direction dir) {
    auto& [ oldaction, olddir ] = playerNextAction;
    oldaction = action;
    olddir = dir;
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

void Chamber::spawn_all(std::shared_ptr<Player>&& playerptr) {
    for (auto& cell : grid) {
        if (cell.get_type() == '.'
            || cell.get_type() == '+'
            || cell.get_type() == '#') {
            cell.unoccupy();
        }
    }
    items.clear();
    citems.clear();
    enemies.clear();

    // 1. number chambers
    std::vector<vecOfCoords> chambers;
    CellArray tempgrid = grid;

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

    if (playerptr) {
        player = std::move(playerptr);
        player->set_pos(std::move(chambers[0].back()));
    } else {
        player = Player::make_player(race, std::move(chambers[0].back()));
    }
    chambers[0].pop_back();

    // 3. spawn stairs
    int numChambers = chambers.size();
    if (numChambers < 2) {
        std::cerr << "Error: Layout must have at least two chambers\n";
        throw "notEnoughChambers";
    }
    std::unique_ptr<ContactItem> stairs
        = ContactItem::make_contact_item('s', std::move(chambers[1].back()), false);
    citems.insert({chambers[1].back(), std::move(stairs)});
    chambers[1].pop_back();

    // 4. spawn 10 potions
    std::default_random_engine& rng{ChamberSettings::get_generator()};
    for (int i = 0; i < 10; ++i) {
        if (numChambers == 0) break;
        int targetChamber = rng() % numChambers;
        const auto [ x, y ] = chambers[targetChamber].back();
        grid[y * ChamberSettings::width() + x].occupy();

        items.insert(
            { chambers[targetChamber].back(),
                Item::make_item('p', std::move(chambers[targetChamber].back())) }
        );

        chambers[targetChamber].pop_back();
        if (chambers[targetChamber].empty()) {
            std::swap(chambers[targetChamber], chambers.back());
            chambers.pop_back();
            --numChambers;
        }
    }

    // 5. spawn 10 piles of gold
    for (int i = 0; i < 10; ++i) {
        if (numChambers == 0) break;
        int targetChamber = rng() % numChambers;

        citems.insert(
            { chambers[targetChamber].back(),
                ContactItem::make_contact_item('g', std::move(chambers[targetChamber].back()), false) }
        );

        chambers[targetChamber].pop_back();
        if (chambers[targetChamber].empty()) {
            std::swap(chambers[targetChamber], chambers.back());
            chambers.pop_back();
            --numChambers;
        }
    }

    // 6. spawn 20 enemies
    for (int i = 0; i < 20; ++i) {
        if (numChambers == 0) break;
        int targetChamber = rng() % numChambers;
        const auto [ x, y ] = chambers[targetChamber].back();
        grid[y * ChamberSettings::width() + x].occupy();

        enemies.emplace_back(
            Enemy::make_enemy(std::move(chambers[targetChamber].back()))
        );

        chambers[targetChamber].pop_back();
        if (chambers[targetChamber].empty()) {
            std::swap(chambers[targetChamber], chambers.back());
            chambers.pop_back();
            --numChambers;
        }
    }
}

void Chamber::next_turn() {
    auto [ action, dir ] = playerNextAction;
    const auto [ playerx, playery ] = player->get_pos();
    Cell& currentCell = grid[playery * ChamberSettings::width() + playerx];
    Cell& targetCell = cell_in_dir(playerx, playery, dir);

    if (action == 'm') {
        currentCell.unoccupy();
        if (targetCell.is_occupied()) {
            dir = Direction::X;
        }
        player->move(dir);
        targetCell.occupy();
        
        // use ContactItems
        auto it = citems.find(player->get_pos());
        if (it != citems.end()) {
            std::get<1>(*it)->trigger(player);
            citems.erase(player->get_pos());
        }
    } else if (action == 'u') {
        if (!targetCell.is_occupied()) return;
        std::pair<int, int> targetCoords = DirUtils::new_coords(player->get_pos(), dir);
        auto it = items.find(targetCoords);
        if (it != items.end()) {
            std::get<1>(*it)->use(player);
            items.erase(targetCoords);
            targetCell.unoccupy();
        }
    } else if (action == 'a') {
        std::pair<int, int> targetCoords = DirUtils::new_coords(player->get_pos(), dir);
        for (auto it = enemies.rbegin(); it != enemies.rend(); ++it) {
            std::unique_ptr<Enemy>& enemyptr = *it;
            if (enemyptr->get_pos() == targetCoords) {
                auto [ hp, atk, def, race, gold ] = player->get_stats();
                enemyptr->tank(atk);
                if (enemyptr->is_alive()) {
                    std::unique_ptr<ContactItem> droppedItem = enemyptr->drop_item();
                    if (droppedItem) {
                        citems.insert({targetCoords, std::move(droppedItem)});
                    }
                    std::swap(enemyptr, enemies.back());
                    enemies.pop_back();
                    targetCell.unoccupy();
                }
            }
        }
    }

    // call passives on player and enemies, check descend logic
    for (auto& enemyptr : enemies) {
        if (enemyptr->player_in_range(player) && enemyptr->is_hostile()) {
            enemyptr->attempt_attack(player);
        } else {
            enemyptr->passive();
            Direction dir = DirUtils::rand_dir();
            auto [ x, y ] = enemyptr->get_pos();
            if (cell_in_dir(x, y, dir).is_occupied()) {
                dir = Direction::X;
            }
            grid[y * ChamberSettings::width() + x].unoccupy();
            enemyptr->move(dir);
            cell_in_dir(x, y, dir).occupy();
        }
    }
}

void Chamber::print() const noexcept {
    constexpr int totalSize = ChamberSettings::width() * ChamberSettings::height();
    std::array<char, totalSize> display;
    for (int i = 0; i < totalSize; ++i) {
        display[i] = grid[i].get_type();
    }

    // items
    for (const auto& [ pos, citemptr ] : citems) {
        const auto [ x, y ] = pos;
        display[y * ChamberSettings::width() + x] = citemptr->get_icon();
    }

    // contact items
    for (const auto& [ pos, itemptr ] : items) {
        const auto [ x, y ] = pos;
        display[y * ChamberSettings::width() + x] = itemptr->get_icon();
    }

    // enemies
    for (const auto& enemyptr : enemies) {
        const auto [ x, y ] = enemyptr->get_pos();
        display[y * ChamberSettings::width() + x] = enemyptr->get_icon();
    }

    const auto [ playerx, playery ] = player->get_pos();
    display[playery * ChamberSettings::width() + playerx] = '@';

    for (int y = 0; y < ChamberSettings::height(); ++y) {
        for (int x = 0; x < ChamberSettings::width(); ++x) {
            char cell = display[y * ChamberSettings::width() + x];
            if (cell == '@') {
                std::cout << "\e[1;36m@\e[1;0m";
            } else {
                std::cout << cell;
            }
        }
        std::cout << '\n';
    }
}