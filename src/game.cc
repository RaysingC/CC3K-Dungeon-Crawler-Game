#include "game.h"
#include <fstream>
#include <iostream>
#include "direction.h"
#include "race.h"
#include "stats.h"
#include "chamber.h"
#include "player.h"

static std::string extract_layout(const std::string& layoutFile) {
    std::ifstream layoutfs{layoutFile}; // path relative to makefile
    std::string noNewlineLayout;

    char cell;
    while (layoutfs >> std::noskipws >> cell) {
        if (cell != '\n')
            noNewlineLayout.push_back(cell);
    }
    return noNewlineLayout;
}

Game::Game(const std::string& layoutFile)
    : chamber{std::move(extract_layout(layoutFile))} {}

void Game::prompt_race() {
    char r;
    while (std::cin >> r) {
        if (Race::valid_race(r)) {
            chamber.set_race(r);
            return;
        }
    }
}

static std::string action_message(char action, Direction dir) {
    if (action == 'm') {
        return "The Player moves " + DirUtils::dir_to_str(dir);
    } else if (action == 'u') {
        return "The Player uses an item";
    } else if (action == 'a') {
        return "The Player slashes to the " + DirUtils::dir_to_str(dir);
    } else {
        return "";
    }
}

void Game::print(const std::string& actionMessage) const noexcept {
    chamber.print();
    const auto [ hp, atk, def, race, gold ] = chamber.get_playerptr()->get_stats();
    std::cout << "Race: " << Race::full_name(race)
        << " Gold: " << gold << '\n'
        << "HP: " << hp << '\n'
        << "Atk: " << atk << '\n'
        << "Def: " << def << '\n'
        << "Action: " << actionMessage << '\n';
}

void Game::play() {
    prompt_race();
    chamber.spawn_all();
    print("");
    Merchant::pacify_merchants();

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "r") {
            ChamberSettings::floor_number() = 1;
            prompt_race();
            chamber.spawn_all();
            print("");
            Merchant::pacify_merchants();
            continue;
        } else if (cmd == "q") {
            break;
        }

        Direction dir = Direction::X;
        char action = 'm';

        if (DirUtils::valid_dir_input(cmd)) {
            dir = DirUtils::str_input_to_dir(cmd);
            action = 'm';
        } else if (cmd == "a" || cmd == "u") {
            std::string inputDir;
            std::cin >> inputDir;
            if (DirUtils::valid_dir_input(inputDir)) {
                dir = DirUtils::str_input_to_dir(inputDir);
                action = cmd.front();
            } else {
                continue;
            }
        }
        chamber.set_player_action(action, dir);
        chamber.next_turn();

        const auto [ hp, atk, def, race, gold ] = chamber.get_playerptr()->get_stats();
        if (hp == 0 || ChamberSettings::floor_number() == 6) {
            break;
        } else if (ChamberSettings::reset_next_turn()) {
            std::shared_ptr<Player> playerptr = chamber.get_playerptr();
            chamber.spawn_all(std::move(playerptr));

            char floorNumber = static_cast<char>(ChamberSettings::floor_number() + '0');
            std::string actionMessage = "The Player enters floor ";
            actionMessage.push_back(floorNumber);
            print(actionMessage);

            ChamberSettings::reset_next_turn() = false;
            continue;
        }

        print(action_message(action, dir));
    }

    end_game_message();
}

void Game::end_game_message() const noexcept {
    const auto [ hp, atk, def, race, gold ] = chamber.get_playerptr()->get_stats();
    std::cout << "YOU " << (hp != 0 && ChamberSettings::floor_number() == 6 ? "BEAT THE GAME" : "DIED") << " WITH A SCORE OF "
        << (race == 'h' ? gold + ((gold + 1) / 2) : gold) << '\n';
}