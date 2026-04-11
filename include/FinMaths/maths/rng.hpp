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

/**
* @brief Generates a uniform random double in the open interval (0, 1).
*
* Uses the formula \f$ \frac{\mathrm{rng}() + 0.5}{\mathrm{rng.max()} + 1.0}\f$ to
* map the discrete integer output of the Mersenne twister to a continuous range
* specifically avoiding 0.0 and 1.0.
*
* @param rng A reference to a random number engine.
* @return A pseudo-random double in (0, 1).
*
* @note This function is not thread-safe. Synchronisation is the responsibility
* of the caller if the engine is shared.
*/
double randomUniform(std::mt19937& rng);

/**
* @brief Thread-safe generation of a uniform random double in the open interval (0, 1).
*
* Accesses the internal global Mersenne twister engine and passes it into randomUniform.
*
* @return A pseudo-random double in (0, 1).
*
* @note This function is thread-safe since it wraps the call in a std::lock_guard.
*/
double randomUniform();

} // namespcae FinMaths::Maths

#endif
