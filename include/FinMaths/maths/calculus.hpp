#ifndef FINMATHS_CALCULUS_HPP
#define FINMATHS_CALCULUS_HPP

#include <cmath>
#include <concepts>
#include <limits>

namespace FinMaths::Maths {

template<std::invocable<double> F>
double differentiateNumerically(F f, double x) {
    double h = std::sqrt(std::numeric_limits<double>::epsilon()) * (std::abs(x) + 1.0);
    return (f(x + h) - f(x - h)) / (2.0 * h);
}

}  // namespace FinMaths::Maths

#endif
