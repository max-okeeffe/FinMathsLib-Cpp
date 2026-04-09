#include <gtest/gtest.h>

#include <stdexcept>

#include "FinMaths/maths/distributions.hpp"

using namespace FinMaths::Maths;

TEST(NormalCDF, Bounds) {
    for (double x : {0.0, 0.1, 0.5, 1.0, 5.0, 10.0}) {
        EXPECT_GE(normalCDF(x), 0.0);
        EXPECT_LE(normalCDF(x), 1.0);
    }
}

TEST(NormalCDF, ExtremeValue) {
    EXPECT_NEAR(normalCDF(1e10), 1.0, 2e-9);
}

TEST(NormalCDF, Increasing) {
    double x = 0.0;
    double y = 0.1;
    while (x < 5.0) {
        EXPECT_LT(normalCDF(x), normalCDF(y));
        x += 0.1;
        y += 0.1;
    }
}

TEST(NormalCDF, Symmetry) {
    for (double x : {0.0, 0.1, 0.5, 1.0, 10.0, 100.0}) {
        EXPECT_NEAR(normalCDF(-x), 1 - normalCDF(x), 2e-9);
    }
}

TEST(NormalCDF, KnownValues) {
    EXPECT_NEAR(normalCDF(1.95996398454), 0.975, 2e-9);
    EXPECT_NEAR(normalCDF(0), 0.5, 2e-9);
}

TEST(NormalCDFInverse, RightInverse) {
    for (int i = 1; i < 100; i++) {
        double x = i / 100.0;
        EXPECT_NEAR(normalCDF(normalCDFInverse(x)), x, 4e-9);
    }
}

TEST(NormalCDFInverse, LeftInverse) {
    for (int x = -5; x < 6; x++) {
        EXPECT_NEAR(normalCDFInverse(normalCDF(x)), x, 1e-4);
    }
}

TEST(NormalCDFInverse, Domain) {
    for (double x : {-10.0, -0.1, 0.0, 1.0, 1.1, 10.0}) {
        EXPECT_THROW(normalCDFInverse(x), std::domain_error);
    }
}
