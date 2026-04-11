#ifndef FINMATHS_CALCULUS_HPP
#define FINMATHS_CALCULUS_HPP

#include <cmath>
#include <concepts>
#include <limits>
#include <stdexcept>

namespace FinMaths::Maths {

/**
* @brief Numerically differentiates a function at a given point using central differences.
*
* The step size is chosen to scale with x as \f$ h = \sqrt{\varepsilon_\text{machine} (|x| + 1) \f$.
* Using central differences gives the method an accuracy of \f$ O(h^2) \f$.
*
* @tparam F A callable type taking and returning a double.
* @param f The function to differentiate.
* @param x The point at which to evaluate the derivative.
* @return The approximate derivative of f at x.
*
* @note Overloaded standard library functions such as @code std::sin @endcode cannot be passed
* directly into the first argument. Wrap them in a lambda function instead.
*/
template<std::invocable<double> F>
double differentiateNumerically(F f, double x) {
    double h = std::sqrt(std::numeric_limits<double>::epsilon()) * (std::abs(x) + 1.0);
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

/**
* @brief Numerically integrates a function over a bounded interval using Simpson's approximation.
*
* Given an interval \f$[a,b]\f$ and a positive number of points \f$n\f$, let \f$h=(b-a)/n\f$.
* Numerically integrating using the midpoint rule yields an error of \f$O(h^2)\f$,
* whereas Simpson's approximation has an error of \f$O(h^4)\f$.
* If an odd integer is passed into nPoints it will be incremented by 1 since
* Simpson's approximation requires an even number.
*
* @tparam F A callable type taking and returning a double.
* @param f The function to integrate.
* @param a The lower limit of integration.
* @param b The upper limit of integration.
* @param nPoints The number of intermediate points at which to sample the function.
* @throws nvalid_argument If nPoints is not positive.
* @returns The definite integral of f from a to to b.
*/
template<std::invocable<double> F>
double integrate(F f, double a, double b, int nPoints=1000) {
    if (nPoints <= 0) {
        throw std::invalid_argument("integrate: nPoints must be positive.");
    }
    if (a == b) {
        return 0.0;
    }
    if (a > b) {
        return -integrate(f, b, a, nPoints);
    }
    if (nPoints % 2 != 0) {
        nPoints++;
    }

    double h = (b - a) / nPoints;
    double total = f(a) + f(b);
    for (int i = 1; i < nPoints; ++i) {
        double x = a + i * h;
        total += (i % 2 == 0 ? 2.0 : 4.0) * f(x);
    }
    return (h / 3.0) * total;
}

}  // namespace FinMaths::Maths

#endif
