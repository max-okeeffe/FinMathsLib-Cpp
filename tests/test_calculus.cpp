#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

#include "FinMaths/maths/calculus.hpp"

using namespace FinMaths::Maths;

TEST(DifferentiateNumerically, Quadratic) {
    auto f = [](double x) { return x * x; };
    EXPECT_NEAR(differentiateNumerically(f, 3.0), 6.0, 1e-8);
    EXPECT_NEAR(differentiateNumerically(f, 0.0), 0.0, 1e-8);
    EXPECT_NEAR(differentiateNumerically(f, -2.0), -4.0, 1e-8);
}

TEST(DifferentiateNumerically, Sine) {
    auto f = [](double x) { return std::sin(x); };
    EXPECT_NEAR(differentiateNumerically(f, 0.0), 1.0, 1e-8);
    EXPECT_NEAR(differentiateNumerically(f, std::numbers::pi / 2.0), 0.0, 1e-8);
}

TEST(DifferentiateNumerically, Exponential) {
    auto f = [](double x) { return std::exp(x); };
    EXPECT_NEAR(differentiateNumerically(f, 1.0), std::exp(1.0), 1e-8);
    EXPECT_NEAR(differentiateNumerically(f, 0.0), 1.0, 1e-8);
    EXPECT_NEAR(differentiateNumerically(f, -1.0), std::exp(-1.0), 1e-8);
}

TEST(Integrate, Cubic) {
    auto f = [](double x) { return x * x * x; };
    EXPECT_NEAR(integrate(f, 0.0, 1.0), 0.25, 1e-8);
}

TEST(Integrate, Sine) {
    auto f = [](double x) { return std::sin(x); };
    EXPECT_NEAR(integrate(f, 0.0, std::numbers::pi), 2.0, 1e-8);
}

TEST(Integrate, Reciprocal) {
    auto f = [](double x) { return 1.0 / x; };
    EXPECT_NEAR(integrate(f, 1.0, 7.0), std::log(7), 1e-8);
}

TEST(Integrate, Convergence) {
    auto f = [](double x) { return std::exp(x); };
    double coarse = integrate(f, 0.0, 5.0, 1000);
    double fine = integrate(f, 0.0, 5.0, 10000);
    double expected = std::exp(5) - 1.0;
    EXPECT_LT(std::abs(fine - expected), std::abs(coarse - expected));
}

TEST(IntegrateToInfinity, ExponentialDecay) {
    auto f = [](double x) { return std::exp(-x); };
    EXPECT_NEAR(integrateToInfinity(f, 0), 1.0, 1e-8);
}

TEST(IntegrateToInfinity, NormalPDF) {
    auto f = [](double x) { return std::exp(-0.5 * x * x) / std::sqrt(2.0 * std::numbers::pi); };
    EXPECT_NEAR(integrateToInfinity(f, 0), 0.5, 1e-8);
}

TEST(IntegrateToInfinity, Reciprocal) {
    auto f = [](double x) { return 1.0 / (x * x); };
    EXPECT_NEAR(integrateToInfinity(f, 0.5), 2.0, 1e-8);
}

TEST(IntegrateToNegativeInfinity, ExponentialDecay) {
    auto f = [](double x) { return std::exp(x); };
    EXPECT_NEAR(integrateToNegativeInfinity(f, 0), 1.0, 1e-8);
}

TEST(IntegrateOverRealLine, NormalPDF) {
    auto f = [](double x) { return std::exp(-0.5 * x * x) / std::sqrt(2.0 * std::numbers::pi); };
    EXPECT_NEAR(integrateOverRealLine(f), 1.0, 1e-8);
}
