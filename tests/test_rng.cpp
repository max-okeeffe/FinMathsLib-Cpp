#include <gtest/gtest.h>

#include "FinMaths/maths/rng.hpp"

using namespace FinMaths::Maths;

TEST(RNG, True) {
    rng();
    EXPECT_TRUE(true);
}
