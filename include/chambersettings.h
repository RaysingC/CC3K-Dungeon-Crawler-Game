#ifndef CHAMBERSETTINGS_H
#define CHAMBERSETTINGS_H

#include <random>
#include <chrono>

// These are the global settings for the chamber including
//   dimensions and random generation settings

namespace ChamberSettings {
    constexpr int width() { return 79; }
    constexpr int height() { return 25; }
    std::default_random_engine& get_generator() noexcept;

    // descend logic
    int& floor_number() noexcept;
    bool& reset_next_turn() noexcept;
}

#endif