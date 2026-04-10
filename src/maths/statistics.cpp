#include "FinMaths/maths/statistics.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace FinMaths::Maths {

double sum(std::span<const double> data) {
    return std::accumulate(data.begin(), data.end(), 0.0);
}

double mean(std::span<const double> data) {
    if (data.empty()) {
        throw std::domain_error("mean: input data must be non-empty");
    }
    return sum(data) / data.size();
}

double standardDeviation(std::span<const double> data, VarianceType type) {
    if (data.empty()) {
        throw std::domain_error("standardDeviation: input data must be non-empty");
    }

    if (data.size() == 1 && type == VarianceType::Sample) {
        throw std::domain_error(
            "standardDeviation: sample standard deviation undefined for data with a single "
            "element.");
    }

    return std::sqrt(variance(data, type));
}

double variance(std::span<const double> data, VarianceType type) {
    if (data.empty()) {
        throw std::domain_error("variance: input data must be non-empty");
    }

    double meanData = 0.0;
    double ell2Data = 0.0;
    std::size_t n = 0;

    for (double num : data) {
        ++n;
        double diff1 = num - meanData;
        meanData += diff1 / n;
        double diff2 = num - meanData;
        ell2Data += diff1 * diff2;
    }

    if (type == VarianceType::Sample) {
        if (n == 1) {
            throw std::domain_error(
                "variance: sample variance undefined for data with a single element.");
        }
        return ell2Data / (n - 1);
    }
    return ell2Data / n;
}

double max(std::span<const double> data) {
    if (data.empty()) {
        throw std::domain_error("max: input data must be non-empty.");
    }
    return *std::ranges::max_element(data);
}

double min(std::span<const double> data) {
    if (data.empty()) {
        throw std::domain_error("min: input data must be non-empty.");
    }
    return *std::ranges::min_element(data);
}

double percentile(std::span<const double> data, double percentage) {
    if (percentage <= 0 || percentage > 100) {
        throw std::domain_error("percentile: percentage must be between 0 and 100 inclusive.");
    }

    if (data.empty()) {
        throw std::domain_error("percentile: input data must be non-empty");
    }

    std::vector<double> temp(data.begin(), data.end());
    double rankApprox = data.size() * percentage / 100.0;
    auto index = static_cast<std::size_t>(std::ceil(rankApprox)) - 1;
    std::nth_element(temp.begin(), temp.begin() + index, temp.end());
    return temp[index];
}

}  // namespace FinMaths::Maths
