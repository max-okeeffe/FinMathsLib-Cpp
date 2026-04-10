#ifndef FINMATHS_STATISTICS_HPP
#define FINMATHS_STATISTICS_HPP

#include <cstdint>
#include <span>

namespace FinMaths::Maths {

enum class VarianceType : std::uint8_t { Population, Sample };

/**
 * @brief Calculates the sum of a collection of doubles.
 *
 * The sum of an empty collection is zero by convention.
 *
 * @param data A collection of numbers.
 * @return The sum of the elements of data.
 */
double sum(std::span<const double> data);

/**
 * @brief Calculates the mean of a collection of doubles.
 *
 * @param data A collection of numbers.
 * @throws domain_error If data is empty.
 * @return The arithmetic mean of the numbers.
 */
double mean(std::span<const double> data);

/**
 * @brief Calculates the standard deviation of a collection of doubles.
 *
 * Calculates the population standard deviation by default and the sample standard
 * deviation if VarianceType::Sample is passed in as the type argument.
 * Uses the square root of the variance function.
 *
 * @param data A collection of numbers.
 * @param type A VarianceType of Population or Sample.
 * @throws domain_error If data is empty, or if type is sample and data has exactly one element.
 * @return The standard deviation of the numbers.
 */
double standardDeviation(std::span<const double> data,
                         VarianceType type = VarianceType::Population);

/**
 * @brief Calculates the variance of a collection of doubles.
 *
 * Uses Welford's algorithm for a one pass calculation.
 *
 * Calculates the population variance by default and the sample variance
 * if VarianceType::Sample is passed in as the type argument.
 *
 * @param data A collection of numbers.
 * @param type A VarianceType of Population or Sample.
 * @throws domain_error If data is empty.
 * @throws domain_error If type is sample and data has exactly one element.
 * @return The variance of the numbers.
 */
double variance(std::span<const double> data, VarianceType type = VarianceType::Population);

/**
 * @brief Calculates the maximum of a collection of doubles.
 *
 * @param data A collection of numbers.
 * @throws domain_error If data is empty.
 * @return The maximum of the numbers.
 */
double max(std::span<const double> data);

/**
 * @brief Calculates the minimum of a collection of doubles.
 *
 * @param data A collection of numbers.
 * @throws domain_error If data is empty.
 * @return The minimum of the numbers.
 */
double min(std::span<const double> data);

/**
 * @brief Calculates the given percentile of a collection of doubles.
 *
 * Uses the nearest-rank method to calculate the percenile. The data does
 * not need to be sorted when passed in.
 *
 * @param data A collection of numbers.
 * @param percentage The percentile we wish to compute.
 * @throws domain_error If data is empty.
 * @throws domain_error If percentage <= 0 or percentage > 100.
 * @return The percentile of the data at percentage.
 */
double percentile(std::span<const double> data, double percentage);

}  // namespace FinMaths::Maths

#endif
