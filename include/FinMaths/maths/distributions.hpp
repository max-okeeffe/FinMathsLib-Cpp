#ifndef FINMATHS_DISTRIBUTIONS_HPP
#define FINMATHS_DISTRIBUTIONS_HPP

namespace FinMaths::Maths {

/**
 * @brief Computes the cumulative distribution of the standard normal disribution.
 *
 * Uses approximations by Abramowitz and Stegun via Horner's method.
 * Maximum absolute error is roughly \f$2\times 10^{-9} \f$.
 *
 * @param x The value at which to evaluate the CDF.
 * @return A value lying in \f$[0,1]\f$.
 */
double normalCDF(double x);

/**
 * @brief Computes the inverse of the cumulative distribution function of the standard normal
 * distribution.
 *
 * Uses Moro's algorithm via Horner's method.
 * Maximum absolute error is around \f$1 \times 10^{-9}$.
 *
 * @param x The value at which to evaluate the inverse CDF.
 * @throws domain_error If \f$x <= 0\f$ or \f$x >= 1\f$.
 * @return A corresponding z-value.
 */
double normalCDFInverse(double x);

}  // namespace FinMaths::Maths

#endif
