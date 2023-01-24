#include "chambersettings.h"
#include <random>
#include <chrono>

namespace ChamberSettings {
    std::default_random_engine& get_generator() noexcept {
        static unsigned int seed = 0; // std::chrono::system_clock::now().time_since_epoch().count();
        static std::default_random_engine generator{seed};
        return generator;
    }

    int& floor_number() noexcept {
        static int floorNumber{1};
        return floorNumber;
    }
    
    bool& reset_next_turn() noexcept {
        static bool resetNextTurn{false};
        return resetNextTurn;
    }
}