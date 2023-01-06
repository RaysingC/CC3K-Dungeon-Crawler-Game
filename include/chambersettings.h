#ifndef CHAMBERSETTINGS_H
#define CHAMBERSETTINGS_H

#include <random>
#include <chrono>

namespace ChamberSettings {
    // preferring functions to variables because "relative order of initialization
    // for non-local static variables is undefined"
    inline constexpr int width() { return 79; }
    inline constexpr int height() { return 25; }
    unsigned seed = 0;
    std::default_random_engine rng{seed};
}

#endif