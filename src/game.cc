#include "game.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "direction.h"
#include "race.h"
#include "stats.h"
#include "chamber.h"
#include "player.h"

// sends newlines to the chamber constructor :(
static std::string extract_layout(const std::string& layoutFile) {
    std::cout << "Custom file layouts are currently not supported\n";
    std::ifstream layoutfs{"./src/default_grid.txt"}; // makefile is outside of src
    std::ostringstream oss;
    
    if (layoutfs) {
        oss << layoutfs.rdbuf();
    }
    layoutfs.close();
    return oss.str();
}

Game::Game(const std::string& layoutFile)
    : floorNumber{1}, chamberptr{std::make_unique<Chamber>(std::move(extract_layout(layoutFile)))} {}

// consider better exception handling
void Game::prompt_race() {
    char r;
    while (std::cin >> r) {
        if (Race::valid_race(r)) {
            chamberptr->set_race(r);
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
    chamberptr->print();
    const auto& [ hp, atk, def, race, gold ] = chamberptr->player_stats().get_tuple();
    std::cout << "Race: " << Race::full_name(race)
        << " Gold: " << gold << '\n'
        << "HP: " << hp << '\n'
        << "Atk: " << atk << '\n'
        << "Def: " << def << '\n'
        << "Action: " << actionMessage << '\n';
}

void Game::play() {
    prompt_race();
    chamberptr->spawn_all();
    print("");

    // code duplication
    std::string cmd;
    while (std::cin >> cmd) {
        if (DirUtils::valid_dir_input(cmd)) {
            Direction dir = DirUtils::str_input_to_dir(cmd);
            chamberptr->set_player_action('m', dir);
            chamberptr->next_turn();
            print(action_message('m', dir));
        } else if (cmd == "a" || cmd == "u") {
            std::string dir;
            std::cin >> dir;
            if (DirUtils::valid_dir_input(dir)) {
                chamberptr->set_player_action(cmd.front(), DirUtils::str_input_to_dir(dir));
                chamberptr->next_turn();
                print(action_message(cmd.front(), DirUtils::str_input_to_dir(dir)));
            }
        } else if (cmd == "r") {
            floorNumber = 1;
            prompt_race();
            chamberptr->spawn_all();
            print("");
        } else if (cmd == "q") {
            break;
        }
    }

    end_game_message();
}

void Game::end_game_message() const {
    const auto& [ hp, atk, def, race, gold ] = chamberptr->player_stats().get_tuple();
    std::cout << "YOU " << (hp != 0 && floorNumber == 6 ? "BEAT THE GAME" : "DIED") << " WITH A SCORE OF "
        << (race == 'h' ? gold + (gold / 2) : gold) << '\n';
}