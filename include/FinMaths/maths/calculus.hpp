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
 * The step size is chosen to scale with x as \f$ h = \sqrt{\varepsilon_\text{machine}} (|x| + 1)
 * \f$. Using central differences gives the method an accuracy of \f$ O(h^2) \f$.
 *
 * @tparam F A callable type taking and returning a double.
 * @param f The function to differentiate.
 * @param x The point at which to evaluate the derivative.
 * @return The approximate derivative of f at x.
 *
 * @note Overloaded standard library functions such as @code std::sin @endcode cannot be passed
 * directly into the first argument. Wrap them in a lambda function instead.
 */
template <std::invocable<double> F>
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
 * @throws invalid_argument If nPoints is not positive.
 * @return The definite integral of f from a to to b.
 */
template <std::invocable<double> F>
double integrate(F f, double a, double b, int nPoints = 1000) {
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

/**
 * @brief Numerically integrates a function over the half open interval \f$[a,\infty)\f$.
 *
 * We use the substitution x = a + t/(1-t), dx/dt = 1/(1-t)^2 to map the interval
 * [a, inf) to [0, 1) and integrate over a bounded interval. To avoid numerical issues
 * we integrate over [0, 1-eps) where eps = 1e-10.
 *
 * @tparam F A callable type taking and returning a double.
 * @param f The function to integrate.
 * @param a The lower limit of integration.
 * @param nPoints The number of intermediate points at which to sample the function.
 * @throws invalid_argument If nPoints is not positive.
 * @return The improper integral of f from a to infinity.
 */
template <std::invocable<double> F>
double integrateToInfinity(F f, double a, int nPoints = 1000) {
    if (nPoints <= 0) {
        throw std::invalid_argument("integrateToInfinity: nPoints must be positive.");
    }

    auto g = [&f, a](double t) {
        double denom = 1.0 - t;
        double x = a + t / denom;
        return f(x) / (denom * denom);
    };
    double eps = 1e-10;
    return integrate(g, 0.0, 1.0 - eps, nPoints);
}

/**
 * @brief Numerically integrates a function over the half open interval \f$(-\infty,b]\f$.
 *
 * We use the substitution x = -t to reflect f and apply integrateToInfinity on [-b, inf).
 *
 * @tparam F A callable type taking and returning a double.
 * @param f The function to integrate.
 * @param b The upper limit of integration.
 * @param nPoints The number of intermediate points at which to sample the function.
 * @throws invalid_argument If nPoints is not positive.
 * @return The improper integral of f from negative infinity to b.
 */
template <std::invocable<double> F>
double integrateToNegativeInfinity(F f, double b, int nPoints = 1000) {
    if (nPoints <= 0) {
        throw std::invalid_argument("integrateToNegativeInfinity: nPoints must be positive.");
    }

    auto g = [&f](double x) { return f(-x); };
    return integrateToInfinity(g, -b, nPoints);
}

/**
 * @brief Numerically integrates a function over the whole real line.
 *
 * We split the domain into (-inf, 0] and [0, inf) and sum the improper
 * integrals of f over these two domains. The value f(0) may be evaluated
 * twice but has negligible effect on accuracy.
 *
 * @tparam F A callable type taking and returning a double.
 * @param f The function to integrate.
 * @param nPoints The number of intermediate points at which to sample the function.
 * @throws invalid_argument If nPoints is not positive.
 * @return The improper integral of f over the real line.
 */
template <std::invocable<double> F>
double integrateOverRealLine(F f, int nPoints = 1000) {
    if (nPoints <= 0) {
        throw std::invalid_argument("integrateOverRealLine: nPoints must be positive.");
    }
    return integrateToInfinity(f, 0.0, nPoints) + integrateToNegativeInfinity(f, 0.0, nPoints);
}

}  // namespace FinMaths::Maths

#endif
