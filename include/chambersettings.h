#ifndef CHAMBERSETTINGS_H
#define CHAMBERSETTINGS_H

#include <random>
#include <chrono>

// These are the global settings for the chamber including
//   dimensions and random generation settings

namespace ChamberSettings {
    // preferring functions to variables because "relative order of initialization
    // for non-local static variables is undefined"
    inline constexpr int width() { return 79; }
    inline constexpr int height() { return 25; }
    // int generate_random_int() noexcept;
    std::default_random_engine& get_generator() noexcept;
}


#endif