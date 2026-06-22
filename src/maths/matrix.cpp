#include "FinMaths/maths/matrix.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace FinMaths::Maths {

// Constructors

Matrix::Matrix(int nrows, int ncols, double value)
    : nrows(validDimension(nrows, "nrows")),
      ncols(validDimension(ncols, "ncols")),
      data(static_cast<std::size_t>(nrows) * ncols, value) {
}

Matrix::Matrix(int nrows, int ncols, const std::vector<double>& values)
    : nrows(validDimension(nrows, "nrows")), ncols(validDimension(ncols, "ncols")), data(values) {
    if (static_cast<std::size_t>(nrows) * ncols != values.size()) {
        throw std::invalid_argument("Matrix: nrows * ncols must equal values.size()");
    }
}

Matrix::Matrix(double value) : nrows(1), ncols(1), data(1, value) {
}

Matrix::Matrix(const std::vector<double>& values, VectorType type)
    : nrows(type == VectorType::Row ? 1 : validDimension(static_cast<int>(values.size()), "nrows")),
      ncols(type == VectorType::Row ? validDimension(static_cast<int>(values.size()), "ncols") : 1),
      data(values) {
}

Matrix::Matrix(const std::vector<std::vector<double> >& values)
    : nrows(validDimension(static_cast<int>(values.size()), "nrows")),
      ncols(validDimension(static_cast<int>(values.empty() ? 0 : values.front().size()), "ncols")) {
    if (nrows == 0 || ncols == 0) {
        throw std::invalid_argument(
            "Matrix: values must be a non-empty vector of non-empty vectors all of the same size");
    }

    data.reserve(static_cast<std::size_t>(nrows) * ncols);

    for (const std::vector<double>& row : values) {
        if (std::cmp_not_equal(row.size(), ncols)) {
            throw std::invalid_argument(
                "Matrix: values must be a non-empty vector of non-empty vectors all of the same "
                "size");
        }
        data.insert(data.end(), row.begin(), row.end());
    }
}

// Accessors

Matrix Matrix::row(int row) const {
    if (row < 0 || row >= nrows) {
        std::stringstream s;
        s << "Matrix::row: input row number must satisfy 0 <= row < " << nrows;
        throw std::out_of_range(s.str());
    }
    Matrix r(1, ncols);
    r.setRow(0, *this, row);
    return r;
}

Matrix Matrix::col(int col) const {
    if (col < 0 || col >= ncols) {
        std::stringstream s;
        s << "Matrix::col: input column number must satisfy 0 <= col < " << ncols;
        throw std::out_of_range(s.str());
    }
    Matrix c(nrows, 1);
    c.setCol(0, *this, col);
    return c;
}

void Matrix::setRow(int row, const Matrix& other, int otherRow) {
    if (ncols != other.ncols) {
        throw std::invalid_argument("Matrix::setRow: matrices must have same number of columns");
    }
    auto it1 = begin() + flatIndex(row, 0);
    const auto it2 = other.begin() + other.flatIndex(otherRow, 0);
    std::copy(it2, it2 + ncols, it1);
}

void Matrix::setCol(int col, const Matrix& other, int otherCol) {
    if (nrows != other.nrows) {
        throw std::invalid_argument("Matrix::setCol: matrices must have the same number of rows");
    }
    auto it1 = begin() + flatIndex(0, col);
    auto it2 = other.begin() + other.flatIndex(0, otherCol);
    for (int row = 0; row < nrows; ++row) {
        *it1 = *it2;
        it1 += ncols;
        it2 += other.ncols;
    }
}

// Arithmetic operators

Matrix& Matrix::operator+=(double scalar) {
    for (auto& x : data) {
        x += scalar;
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (nrows != other.nrows || ncols != other.ncols) {
        throw std::invalid_argument("Matrix + : matrices must have the same dimensions");
    }
    std::ranges::transform(*this, other, begin(), std::plus<>());
    return *this;
}

Matrix& Matrix::operator-=(double scalar) {
    for (auto& x : data) {
        x -= scalar;
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (nrows != other.nrows || ncols != other.ncols) {
        throw std::invalid_argument("Matrix - : matrices must have the same dimensions");
    }
    std::ranges::transform(*this, other, begin(), std::minus<>());
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (auto& x : data) {
        x *= scalar;
    }
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    if (scalar == 0.0) {
        throw std::invalid_argument("Matrix / : cannot divide by zero");
    }
    return *this *= 1.0 / scalar;
}

// Comparison operators

bool Matrix::operator==(const Matrix& other) const noexcept {
    return nrows == other.nrows && ncols == other.ncols && data == other.data;
}

bool Matrix::isApprox(const Matrix& other, double absTolerance,
                      double relTolerance) const noexcept {
    if (nrows != other.nrows || ncols != other.ncols) {
        return false;
    }

    auto it = other.begin();
    for (const double x : data) {
        const double y = *it;
        const double diff = std::abs(x - y);

        if (diff > absTolerance && diff > relTolerance * std::max(std::abs(x), std::abs(y))) {
            return false;
        }
        ++it;
    }
    return true;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.nCols() != rhs.nRows()) {
        throw std::invalid_argument(
            "Matrix * : matrix multiplication only defined for m by k and k by n matrices.");
    }

    int m = lhs.nRows();
    int n = rhs.nCols();
    int k = lhs.nCols();
    Matrix ret(m, n);

    auto retBegin = ret.begin();
    auto lhsBegin = lhs.begin();

    for (int i = 0; i < m; i++) {
        auto lhsIter = lhsBegin;
        auto rhsBegin = rhs.begin();

        for (int j = 0; j < k; j++) {
            double lhsVal = *lhsIter++;

            auto rhsIter = rhsBegin;
            auto rhsEnd = rhsIter + n;
            auto retIter = retBegin;

            while (rhsIter != rhsEnd) {
                *retIter++ += lhsVal * (*rhsIter++);
            }
            rhsBegin += n;
        }
        lhsBegin += k;
        retBegin += n;
    }
    return ret;
}

}  // namespace FinMaths::Maths
