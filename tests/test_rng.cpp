#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>
#include <numbers>
#include <vector>

#include "FinMaths/maths/distributions.hpp"
#include "FinMaths/maths/rng.hpp"
#include "FinMaths/maths/statistics.hpp"

using namespace FinMaths::Maths;

TEST(SeedRNG, Reproducibility) {
    seedRNG(42);
    double first = randomUniform();
    double second = randomUniform();

    seedRNG(42);
    EXPECT_DOUBLE_EQ(randomUniform(), first);
    EXPECT_DOUBLE_EQ(randomUniform(), second);
}

TEST(SeedRNG, DifferentSeeds) {
    seedRNG(42);
    double a = randomUniform();

    seedRNG(123);
    double b = randomUniform();

    EXPECT_NE(a, b);
}

class RandomUniformTest : public ::testing::Test {
   protected:
    static void SetUpTestSuite() {
        seedRNG(42);
        samples.resize(100000);
        for (double& x : samples) {
            x = randomUniform();
        }
    }

    static std::vector<double> samples;
};

std::vector<double> RandomUniformTest::samples;

TEST_F(RandomUniformTest, Bounds) {
    for (double x : samples) {
        EXPECT_GT(x, 0.0);
        EXPECT_LT(x, 1.0);
    }
}

TEST_F(RandomUniformTest, Mean) {
    EXPECT_NEAR(mean(samples), 0.5, 0.01);
}

TEST_F(RandomUniformTest, Variance) {
    EXPECT_NEAR(variance(samples), 1.0 / 12.0, 0.01);
}

TEST_F(RandomUniformTest, IntervalProportion) {
    double a = 1.0 / std::numbers::sqrt3;
    double b = 1.0 / std::numbers::sqrt2;

    int count = std::ranges::count_if(samples, [a, b](double x) { return x > a && x < b; });
    EXPECT_NEAR(static_cast<double>(count) / samples.size(), b - a, 0.01);
}

class RandomNormalTest : public ::testing::Test {
   protected:
    static void SetUpTestSuite() {
        seedRNG(42);
        samples.resize(100000);
        for (double& x : samples) {
            x = randomNormal();
        }
    }

    static std::vector<double> samples;
};

std::vector<double> RandomNormalTest::samples;

TEST_F(RandomNormalTest, Mean) {
    EXPECT_NEAR(mean(samples), 0.0, 0.01);
}

TEST_F(RandomNormalTest, Variance) {
    EXPECT_NEAR(variance(samples), 1.0, 0.01);
}

TEST_F(RandomNormalTest, IntervalProportion) {
    double a = 1.0 / std::numbers::sqrt3;
    double b = 1.0 / std::numbers::sqrt2;

    int count = std::ranges::count_if(
        samples, [a, b](double x) { return std::abs(x) > a && std::abs(x) < b; });

    EXPECT_NEAR(static_cast<double>(count) / samples.size(), 2.0 * (normalCDF(b) - normalCDF(a)),
                0.01);
}
