#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "FinMaths/maths/statistics.hpp"

using namespace FinMaths::Maths;

class StatisticsTest : public ::testing::Test {
   protected:
    std::vector<double> empty;
    std::vector<double> singleton = {21.0};
    std::vector<double> hundredNumbers;

    void SetUp() override {
        for (int i = 1; i <= 100; i++) {
            hundredNumbers.push_back(static_cast<double>(i));
        }
    }
};

TEST(Sum, VariousContainers) {
    const std::vector<double> vec = {1.0, 2.0, 4.0};
    const std::array<double, 3> arr = {1.0, 2.0, 4.0};
    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    double raw[] = {1.0, 2.0, 4.0};

    EXPECT_DOUBLE_EQ(sum(vec), 7.0);
    EXPECT_DOUBLE_EQ(sum(arr), 7.0);
    EXPECT_DOUBLE_EQ(sum(raw), 7.0);
}

TEST(Sum, Subspans) {
    std::vector<double> v = {100.0, 1.0, 2.0, 4.0, 100.0};
    std::span<double> middle(v.data() + 1, 3);

    EXPECT_DOUBLE_EQ(sum(middle), 7.0);
}

TEST_F(StatisticsTest, SumEmpty) {
    EXPECT_DOUBLE_EQ(sum(empty), 0.0);
}

TEST_F(StatisticsTest, SumDiscreteUniform) {
    EXPECT_DOUBLE_EQ(sum(hundredNumbers), 5050);
}

TEST_F(StatisticsTest, MeanDiscreteUniform) {
    EXPECT_DOUBLE_EQ(mean(hundredNumbers), 50.5);
}

TEST_F(StatisticsTest, MeanEmpty) {
    EXPECT_THROW(mean(empty), std::domain_error);
}

TEST_F(StatisticsTest, VarianceDiscreteUniform) {
    EXPECT_DOUBLE_EQ(variance(hundredNumbers, VarianceType::Population), 833.25);
    EXPECT_DOUBLE_EQ(variance(hundredNumbers, VarianceType::Sample), 2525.0 / 3.0);
}

TEST_F(StatisticsTest, VarianceDefault) {
    EXPECT_DOUBLE_EQ(variance(hundredNumbers), 833.25);
}

TEST_F(StatisticsTest, VarianceEmpty) {
    EXPECT_THROW(variance(empty), std::domain_error);
    EXPECT_THROW(variance(empty, VarianceType::Sample), std::domain_error);
}

TEST_F(StatisticsTest, VarianceSingleton) {
    EXPECT_DOUBLE_EQ(variance(singleton), 0.0);
    EXPECT_THROW(variance(singleton, VarianceType::Sample), std::domain_error);
}

TEST_F(StatisticsTest, StandardDeviationDiscreteUniform) {
    EXPECT_DOUBLE_EQ(standardDeviation(hundredNumbers, VarianceType::Population),
                     std::sqrt(833.25));
    EXPECT_DOUBLE_EQ(standardDeviation(hundredNumbers, VarianceType::Sample),
                     std::sqrt(2525.0 / 3.0));
}

TEST_F(StatisticsTest, StandardDeviationDefault) {
    EXPECT_DOUBLE_EQ(standardDeviation(hundredNumbers), std::sqrt(833.25));
}

TEST_F(StatisticsTest, StandardDeviationEmpty) {
    EXPECT_THROW(standardDeviation(empty), std::domain_error);
    EXPECT_THROW(standardDeviation(empty, VarianceType::Sample), std::domain_error);
}

TEST_F(StatisticsTest, StandardDeviationSingleton) {
    EXPECT_DOUBLE_EQ(standardDeviation(singleton), 0.0);
    EXPECT_THROW(standardDeviation(singleton, VarianceType::Sample), std::domain_error);
}

TEST_F(StatisticsTest, MaxHundred) {
    EXPECT_DOUBLE_EQ(max(hundredNumbers), 100.0);
}

TEST_F(StatisticsTest, MaxEmpty) {
    EXPECT_THROW(max(empty), std::domain_error);
}

TEST_F(StatisticsTest, MinHundred) {
    EXPECT_DOUBLE_EQ(min(hundredNumbers), 1.0);
}

TEST_F(StatisticsTest, MinEmpty) {
    EXPECT_THROW(min(empty), std::domain_error);
}

TEST_F(StatisticsTest, PercentileCeil) {
    for (int i = 1; i <= 1000; i++) {
        double percentage = i / 10.0;
        EXPECT_DOUBLE_EQ(percentile(hundredNumbers, percentage), std::ceil(percentage));
    }
}

TEST_F(StatisticsTest, PercentileUnordered) {
    std::vector<double> v = {1.0, 7.0, 4.0, 2.0, 6.0};
    EXPECT_DOUBLE_EQ(percentile(v, 73.2), 6.0);
}

TEST_F(StatisticsTest, PercentileSubspan) {
    std::vector<double> v = {3.0, 1.0, 7.0, 4.0, 2.0, 6.0, 5.0};
    std::span<const double> middle(v.data() + 1, 5);
    EXPECT_DOUBLE_EQ(percentile(middle, 73.2), 6.0);
}

TEST_F(StatisticsTest, PercentileOutOfBounds) {
    EXPECT_THROW(percentile(hundredNumbers, -0.1), std::domain_error);
    EXPECT_THROW(percentile(hundredNumbers, 0.0), std::domain_error);
    EXPECT_THROW(percentile(hundredNumbers, 100.1), std::domain_error);
}

TEST_F(StatisticsTest, PercentileEmpty) {
    EXPECT_THROW(percentile(empty, 50.0), std::domain_error);
}
