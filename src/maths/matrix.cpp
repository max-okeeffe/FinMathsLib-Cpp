#include "FinMaths/maths/matrix.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <sstream>
#include <stdexcept>
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

// Comparison operators

bool Matrix::operator==(const Matrix& other) const noexcept {
    return nrows == other.nrows && ncols == other.ncols && data == other.data;
}

bool Matrix::isApprox(const Matrix& other, double tolerance) const noexcept {
    if (nrows != other.nrows || ncols != other.ncols) {
        return false;
    }

    auto it2 = other.begin();
    auto end = this->end();

    for (auto it1 = this->begin(); it1 != end; ++it1) {
        if (std::abs(*it1 - *it2) > tolerance) {
            return false;
        }
        ++it2;
    }
    return true;
}

}  // namespace FinMaths::Maths
