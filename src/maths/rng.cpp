#include "FinMaths/maths/rng.hpp"

#include <mutex>
#include <random>

namespace FinMaths::Maths {

namespace {

std::mt19937 mersenneTwister;
std::mutex rngMutex;

}

void seedRNG(unsigned int seed) {
    std::lock_guard<std::mutex> lock(rngMutex);
    mersenneTwister.seed(seed);
}

} // namespace FinMaths::Maths
