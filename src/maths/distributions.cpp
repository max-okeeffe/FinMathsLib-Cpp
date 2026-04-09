#include "FinMaths/maths/distributions.hpp"

#include <array>
#include <cmath>
#include <stdexcept>

namespace FinMaths {
namespace Maths {

namespace {

// clang-format off
constexpr std::array<double, 8> normA = {
    0.0,
    0.316879890481381,
    -0.138329314150635,
    1.08680830347054,
    -1.11694155120396,
    1.20644903073232,
    -0.393127715207728,
    0.0382613542530727,
};

constexpr std::array<double, 3> normP = {
    1.0,
    0.406742016006509,
    0.0072279182302319
};

constexpr std::array<double, 4> invA = {
    2.50662823884,
    -18.61500062529,
    41.39119773534,
    -25.44106049637
};

constexpr std::array<double, 5> invB = {
    1.0,
    -8.47351093090,
    23.08336743743,
    -21.06224101826,
    3.13082909833
};

constexpr std::array<double, 9> invC = {
    0.3374754822726147,
    0.9761690190917186,
    0.1607979714918209,
    0.0276438810333863,
    0.0038405729373609,
    0.0003951896511919,
    0.0000321767881768,
    0.0000002888167364,
    0.0000003960315187
};
// clang-format on

template <std::size_t N>
inline double horner(double x, const std::array<double, N>& coeffs) {
    double ret = 0;
    for (auto it = coeffs.rbegin(); it != coeffs.rend(); it++) {
        ret = *it + x * ret;
    }
    return ret;
}

}  // namespace

double normalCDF(double x) {
    if (x == 0) {
        return 0.5;
    }
    if (x < 0) {
        return 1 - normalCDF(-x);
    }
    double t = 1 / horner(x * M_SQRT1_2, normP);
    return 1 - 0.5 * std::exp(-0.5 * x * x) * horner(t, normA);
}

double normalCDFInverse(double x) {
    if (x <= 0 || x >= 1) {
        throw std::domain_error("normalCDFInverse: input must be strictly between 0 and 1");
    }

    double y = x - 0.5;
    if (std::abs(y) < 0.42) {
        double r = y * y;
        return y * horner(r, invA) / horner(r, invB);
    }

    double r = y < 0 ? x : 1 - x;
    double s = std::log(-std::log(r));
    double t = horner(s, invC);
    return x > 0.5 ? t : -t;
}

}  // namespace Maths
}  // namespace FinMaths
