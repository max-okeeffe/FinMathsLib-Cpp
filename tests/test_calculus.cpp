#include <gtest/gtest.h>

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
