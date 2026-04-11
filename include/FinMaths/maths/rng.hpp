#ifndef FINMATHS_RNG_HPP
#define FINMATHS_RNG_HPP

#include <random>

namespace FinMaths::Maths {

/**
* @brief Reseeds the global pseudo-random number generator.
*
* Sets the starting state for the internal Mersenne twister engine (std::mt19937).
*
* @param seed The value used to initialise the RNG state.
* 
* @note This function is thread-safe. It uses a std::mutex to prevent race conditions. */
void seedRNG(unsigned int seed=std::mt19937::default_seed);

} // namespcae FinMaths::Maths

#endif
