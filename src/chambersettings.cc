#include "chambersettings.h"
#include <random>
#include <chrono>

namespace ChamberSettings {
    /*
    int generate_random_int() noexcept {
        unsigned seed = 0;
        static std::default_random_engine generator{seed};
        return generator();
    }
    */
    std::default_random_engine& get_generator() noexcept {
        static unsigned int seed = 0; // std::chrono::system_clock::now().time_since_epoch().count();
        static std::default_random_engine generator{seed};
        return generator;
    }
}