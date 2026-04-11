#include "FinMaths/maths/rng.hpp"

#include <mutex>
#include <random>

#include "FinMaths/maths/distributions.hpp"

namespace FinMaths::Maths {

namespace {

std::mt19937 mersenneTwister;
std::mutex rngMutex;

}  // namespace

void seedRNG(unsigned int seed) {
    std::lock_guard<std::mutex> lock(rngMutex);
    mersenneTwister.seed(seed);
}

double randomUniform(std::mt19937& rng) {
    return (static_cast<double>(rng()) + 0.5) / (static_cast<double>(std::mt19937::max()) + 1.0);
}

double randomUniform() {
    std::lock_guard<std::mutex> lock(rngMutex);
    return randomUniform(mersenneTwister);
}

double randomNormal(std::mt19937& rng) {
    return normalCDFInverse(randomUniform(rng));
}

double randomNormal() {
    std::lock_guard<std::mutex> lock(rngMutex);
    return randomNormal(mersenneTwister);
}

}  // namespace FinMaths::Maths
