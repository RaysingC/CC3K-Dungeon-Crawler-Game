#ifndef RACE_H
#define RACE_H

#include <string>

namespace Race {
    bool valid_race(char race) noexcept {
        const std::string validRaces{"hedo"};
        for (char validRace : validRaces) {
            if (race == validRace) return true;
        }
        return false;
    }

    std::string full_name(char race) noexcept {
        if (race == 'h') {
            return "Human";
        } else if (race == 'e') {
            return "Elf";
        } else if (race == 'd') {
            return "Dwarf";
        } else if (race == 'o') {
            return "Orc";
        } else {
            return std::string{race};
        }
    }
}


#endif