#include <gtest/gtest.h>

#include "FinMaths/maths/calculus.hpp"

using namespace FinMaths::Maths;

TEST(DifferentiateNumerically, Pass) {
    EXPECT_NO_THROW(differentiateNumerically());
}
