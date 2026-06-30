#ifndef FINMATHS_MATRIX_HPP
#define FINMATHS_MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <ostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace FinMaths::Maths {

enum class VectorType : std::uint8_t { Row, Column };

/**
 * @class Matrix
 * @brief Dense matrix class provides basic linear algebra operations.
 * including arithmetic, element-wise transformations, and matrix multiplication.
 *
 * Storage is contiguous in memory and optimised for numerical operations.
 *
 * @note Indices are zero-based.
 * @note Elements are stored in row-major order.
 * @note Comparison operators are performed elementwise and return a matrix of 1.0 if
 * the comparison is true and 0.0 if it is false. Matrices consisting only of 1.0 and 0.0
 * will be called "truth" matrices.
 */
class Matrix {
   public:
    // Constructors

    /**
     * @brief Construct a matrix with the given dimensions and inital value.
     *
     * @param nrows Number of rows.
     * @param ncols Number of columns.
     * @param value Initial value for all elements.
     *
     * @throw std::invalid_argument if dimensions are not positive.
     */
    Matrix(int nrows, int ncols, double value = 0.0);

    /**
     * @brief Construct a matrix with the given dimensions and input data.
     *
     * @param nrows Number of rows.
     * @param ncols Number of columns.
     * @param values Elements of the matrix
     *
     * @throw std::invalid_argument if dimensions are not positive or nrows * ncols != values.size()
     */
    Matrix(int nrows, int ncols, std::vector<double> values);

    /**
     * @brief Construct a 1x1 matrix with the given value.
     *
     * @param value The singleton data.
     */
    explicit Matrix(double value);

    /**
     * @brief Construct a row or column matrix from a vector.
     *
     * @param values Elements of the matrix.
     * @param type VectorType Row or Column for the resulting matrix.
     *
     * @throw std::invalid_argument if values is empty.
     */
    explicit Matrix(std::vector<double> values, VectorType type = VectorType::Row);

    /**
     * @brief Construct a matrix from a vector of vectors.
     *
     * @param values a vector of vector of doubles.
     *
     * @throw std::invalid_argument if values is empty or not all elements of values have the same
     * positive size.
     */
    explicit Matrix(const std::vector<std::vector<double> >& values);

    /**
     * @brief Returns an identity matrix of a given size.
     *
     * @param size The dimensions of the identity matrix.
     * @returns A size by size identity matrix.
     *
     * @throws invalid_argument if size is not positive.
     */
    [[nodiscard]] static Matrix identity(int size);

    // Accessors

    /**
     * @brief Return the number of rows of the matrix.
     */
    [[nodiscard]] int nRows() const noexcept {
        return nrows;
    }
    /**
     * @brief Return the number of columns of the matrix.
     */
    [[nodiscard]] int nCols() const noexcept {
        return ncols;
    }
    /**
     * @brief Return the size of the matrix calculated as nrows * ncols.
     */
    [[nodiscard]] int size() const noexcept {
        return nrows * ncols;
    }
    /**
     * @brief Return the element of the matrix at index (i, j).
     *
     * @param i Row index.
     * @param j Column index.
     *
     * @return The value of the matrix at index (i, j). */
    [[nodiscard]] double get(int i, int j) const {
        return data[flatIndex(i, j)];
    }

    /**
     * @brief Return an iterator to the beginning of the matrix data.
     */
    [[nodiscard]] std::vector<double>::iterator begin() noexcept {
        return data.begin();
    }
    /**
     * @brief Return an iterator to the end of the matrix data.
     */
    [[nodiscard]] std::vector<double>::iterator end() noexcept {
        return data.end();
    }
    /**
     * @brief Return a const iterator to the beginning of the matrix data.
     */
    [[nodiscard]] std::vector<double>::const_iterator begin() const noexcept {
        return data.begin();
    }
    /**
     * @brief Return a const iterator to the end of the matrix data.
     */
    [[nodiscard]] std::vector<double>::const_iterator end() const noexcept {
        return data.end();
    }

    /**
     * @brief Return one of a matrix's rows.
     *
     * @param row Row index.
     * @throw std::out_of_range if row is negative or bigger than nrows.
     * @return A row matrix of the corresponding row.
     */
    [[nodiscard]] Matrix row(int row) const;

    /**
     * @brief Return one of a matrix's columns.
     *
     * @param col Column index.
     * @throw std::out_of_range if col is negative or bigger than ncols.
     * @return A column matrix of the corresponding column.
     */
    [[nodiscard]] Matrix col(int col) const;

    /**
     * @brief Set one of a matrix's rows to be equal to the row of another.
     *
     * @param row The row index of this matrix.
     * @param other Another matrix to be copied from.
     * @param otherRow The row index of the other matrix to be copied.
     * @throw std::invalid_argument if the two matrices have different row lengths.
     *
     * @note Row index bounds checking is done indirectly through flatIndex.
     */
    void setRow(int row, const Matrix& other, int otherRow);

    /**
     * @brief Set one of a matrix's columns to be equal to the column of another.
     *
     * @param col The column index of this matrix.
     * @param other Another matrix to be copied from.
     * @param otherCol The column index of the other matrix to be copied.
     * @throw std::invalid_argument if the two matrices have different column lengths.
     *
     * @note Column index bounds checking is done indirectly through flatIndex.
     */
    void setCol(int col, const Matrix& other, int otherCol);

    /**
     * @brief Access element (i, j) with bounds checking.
     *
     * @param i Row index.
     * @param j Column index.
     * @return A reference to the element at index (i, j).
     *
     * @note Performs bounds checking through flatIndex.
     */
    double& at(int i, int j) {
        return data[flatIndex(i, j)];
    }

    /**
     * @brief Access element (i, j) with bounds checking.
     *
     * @param i Row index.
     * @param j Column index.
     * @return A const reference to the element at index (i, j).
     *
     * @note Performs bounds checking through flatIndex.
     */
    [[nodiscard]] const double& at(int i, int j) const {
        return data[flatIndex(i, j)];
    }

    /**
     * @brief Unchecked access of element (i, j) for faster access.
     *
     * @param i Row index.
     * @param j Column index.
     * @return A reference to the element at index (i, j).
     *
     * @warning No bounds checking is performed.
     */
    double& operator()(int i, int j) noexcept {
        return data[i * ncols + j];
    }

    /**
     * @brief Unchecked access of element (i, j) for faster access.
     *
     * @param i Row index.
     * @param j Column index.
     * @return A const reference to the element at index (i, j).
     *
     * @warning No bounds checking is performed.
     */
    const double& operator()(int i, int j) const noexcept {
        return data[i * ncols + j];
    }

    // Converters

    /**
     * @brief Converts a 1-dimensional matrix to a vector.
     *
     * No copy overhead and is therefore efficient for both row and column matrices.
     *
     * @throw std::invalid_argument if nrows != 1 and ncols != 1.
     * @return A constant reference to the matrix's data.
     */
    [[nodiscard]] const std::vector<double>& asVector() const {
        if (nrows != 1 && ncols != 1) {
            throw std::invalid_argument("asVector: matrix must be a row or column matrix");
        }
        return data;
    }

    /**
     * @brief Converts a 1-dimensional matrix to a std::span.
     *
     * Creates a non-owning read-only view of the matrix's data.
     * This is faster than returning a copy of the data as a vector.
     *
     * @throw std::invalid_argument if nrows != 1 and ncols != 1.
     * @return A std::span<const double> corresponding to the data.
     */
    [[nodiscard]] std::span<const double> asSpan() const {
        if (nrows != 1 && ncols != 1) {
            throw std::invalid_argument("asSpan: matrix must be a row or column matrix");
        }
        return data;
    }

    /**
     * @brief Converts a 1x1 matrix to a scalar.
     *
     * @throw std::invalid_argument if nrows != 1 or ncols != 1.
     * @return A double of the matrix's single element.
     */
    [[nodiscard]] double asScalar() const {
        if (nrows != 1 || ncols != 1) {
            throw std::invalid_argument("asScalar: matrix must be 1x1");
        }
        return data.front();
    }

    // Arithmetic operators

    /**
     * @brief Adds scalar to the elements of this.
     */
    Matrix& operator+=(double scalar);

    /**
     * @brief Adds a matrix other elementwise to this.
     * @throw std::invalid_argument if other has different dimensions from this.
     */
    Matrix& operator+=(const Matrix& other);

    /**
     * @brief Takes away a scalar from the elements of this.
     */
    Matrix& operator-=(double scalar);

    /**
     * @brief Takes away a matrix other elementwise from this.
     * @throw std::invalid_argument if other has different dimensions from this.
     */
    Matrix& operator-=(const Matrix& other);

    /**
     * @brief Multiplies the elements of this by scalar.
     */
    Matrix& operator*=(double scalar);

    /**
     * @brief Divides the elements of this by scalar.
     * @throw std::invalid_argument if scalar equals zero.
     */
    Matrix& operator/=(double scalar);

    /**
     * @brief Adds a matrix and a scalar.
     */
    friend Matrix operator+(Matrix m, double scalar) {
        m += scalar;
        return m;
    }

    /**
     * @brief Adds a scalar and a matrix.
     */
    friend Matrix operator+(double scalar, Matrix m) {
        m += scalar;
        return m;
    }

    /**
     * @brief Adds two matrices together.
     * @throw std::invalid_argument if the matrices do not have the same dimensions.
     */
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
        lhs += rhs;
        return lhs;
    }

    /**
     * @brief Returns the additive inverse of a matrix.
     */
    friend Matrix operator-(Matrix m) {
        std::ranges::transform(m, m.begin(), std::negate<>());
        return m;
    }

    /**
     * @brief Subtracts a scalar from a matrix.
     */
    friend Matrix operator-(Matrix m, double scalar) {
        m -= scalar;
        return m;
    }

    /**
     * @brief Subtracts a matrix from a scalar.
     */
    friend Matrix operator-(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(), [scalar](double x) { return scalar - x; });
        return m;
    }

    /**
     * @brief Subtracts one matrix from another.
     * @throw std::invalid_argument if the matrices have different dimensions.
     */
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
        lhs -= rhs;
        return lhs;
    }

    /**
     * @brief Multiplies a matrix by a scalar.
     */
    friend Matrix operator*(Matrix m, double scalar) {
        m *= scalar;
        return m;
    }

    /**
     * @brief Multiplies a scalar by a matrix.
     */
    friend Matrix operator*(double scalar, Matrix m) {
        m *= scalar;
        return m;
    }

    /**
     * @brief Divides a matrix by a scalar.
     * @throw std::logic_error if scalar is zero.
     */
    friend Matrix operator/(Matrix m, double scalar) {
        m /= scalar;
        return m;
    }

    /**
     * @brief Calculates the trace of the matrix.
     */
    [[nodiscard]] double trace() const;

    /**
     * @brief Returns the transpose of a matrix.
     */
    [[nodiscard]] Matrix transpose() const;

    /**
     * @brief Returns whether or not the matrix is square.
     */
    [[nodiscard]] bool isSquare() const {
        return nrows == ncols;
    }

    /**
     * @brief Returns whether or not the matrix is symmetric.
     */
    [[nodiscard]] bool isSymmetric() const;

    // Elementwise operators

    /**
     * @brief Returns a copy of the matrix with a function applied elementwise.
     */
    template <std::invocable<double> F>
    [[nodiscard]] Matrix apply(F f) const {
        Matrix result(*this);
        std::ranges::transform(result, result.begin(), f);
        return result;
    }

    /**
     * @brief Applies the exponential function elementwise.
     */
    [[nodiscard]] Matrix exp() const {
        return apply([](double x) { return std::exp(x); });
    }

    /**
     * @brief Applies the logarithm function elementwise.
     */
    [[nodiscard]] Matrix log() const {
        return apply([](double x) { return std::log(x); });
    }

    /**
     * @brief Applies the square root function elementwise.
     */
    [[nodiscard]] Matrix sqrt() const {
        return apply([](double x) { return std::sqrt(x); });
    }

    /**
     * @brief Applies the function max(x, 0.0) elementwise.
     */
    [[nodiscard]] Matrix positivePart() const {
        return apply([](double x) { return std::max(x, 0.0); });
    }

    /**
     * @brief Applies the function min(x, 0.0) elementwise.
     */
    [[nodiscard]] Matrix negativePart() const {
        return apply([](double x) { return std::min(x, 0.0); });
    }

    /**
     * @brief Applies the power function elementwise with a given exponent.
     *
     * @param double exponent The power to which each element will be taken.
     */
    [[nodiscard]] Matrix pow(double exponent) const {
        return apply([exponent](double x) { return std::pow(x, exponent); });
    }

    /**
     * @brief Applies the power function elementwise with a given matrix.
     *
     * @param Matrix exponent The power to which each element will be taken.
     * @throw std::invalid_argument if the matrices have different dimensions.
     */
    [[nodiscard]] Matrix pow(const Matrix& exponent) const {
        if (nrows != exponent.nrows || ncols != exponent.ncols) {
            throw std::invalid_argument(
                "Matrix::pow: matrix and exponent matrix must have the same dimensions");
        }

        Matrix result(*this);
        std::ranges::transform(result, exponent, result.begin(),
                               [](double x, double p) { return std::pow(x, p); });
        return result;
    }

    /**
     * @brief Applies multiplication elementwise with a given matrix.
     *
     * @param Matrix other The matrix whose elements will be multiplied with.
     * @throw std::invalid_argument if the matrices have different dimensions.
     */
    [[nodiscard]] Matrix times(const Matrix& other) const {
        if (nrows != other.nrows || ncols != other.ncols) {
            throw std::invalid_argument(
                "Matrix::times: matrix and other matrix must have the same dimensions");
        }

        Matrix result(*this);
        std::ranges::transform(result, other, result.begin(), std::multiplies<>());
        return result;
    }

    // Comparison operators

    /**
     * @brief Tests whether two matrices are equal.
     *
     * Implemented by ensuring dimensions and elements are equal.
     */
    [[nodiscard]] bool equals(const Matrix& other) const noexcept {
        return nrows == other.nrows && ncols == other.ncols && data == other.data;
    }

    /**
     * @brief Tests whether two matrices are equal within an absolute and relative
     * tolerance.
     *
     * Implemented by ensuring dimensions are equal and elements x, y are
     * either less than each other in absolute tolerance or
     * |x - y| <= relTolerance * max(|x|, |y|).
     *
     * @param other Matrix to compare against.
     * @param absTolerance Absolute tolerance.
     * @param relTolerance Relative tolerance.
     * @return True if the matrices are approximately equal and false otherwise.
     */
    [[nodiscard]] bool isApprox(const Matrix& other, double absTolerance = 1e-12,
                                double relTolerance = 1e-12) const noexcept;

    /**
     * @brief Performs an elementwise greater than comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator>(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix >: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::greater<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise greater than comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator>(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(), [scalar](double x) { return x > scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise greater than comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator>(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(), [scalar](double x) { return scalar > x ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise greater than or equal comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator>=(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix >=: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::greater_equal<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise greater than or equal comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator>=(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return x >= scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise greater than or equal to comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator>=(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return scalar >= x ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise less than comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator<(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix <: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::less<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise less than comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator<(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(), [scalar](double x) { return x < scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise less than comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator<(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(), [scalar](double x) { return scalar < x ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise less than or equal comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator<=(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix <=: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::less_equal<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise less than or equal comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator<=(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return x <= scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise less than or equal to comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator<=(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return scalar <= x ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise equal to comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator==(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix ==: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::equal_to<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise equal to comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator==(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return x == scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise equal to comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator==(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return scalar == x ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise not equal to comparison of two matrices.
     *
     * @param lhs Left hand matrix.
     * @param rhs Right hand matrix.
     * @throw std::invalid_argument if x and y have different dimensions.
     * @return A truth matrix.
     */
    friend Matrix operator!=(Matrix lhs, const Matrix& rhs) {
        if (lhs.nrows != rhs.nrows || lhs.ncols != rhs.ncols) {
            throw std::invalid_argument("Matrix !=: matrices must have the same dimension");
        }

        std::ranges::transform(lhs, rhs, lhs.begin(), std::not_equal_to<>());
        return lhs;
    }

    /**
     * @brief Performs an elementwise not equal to comparison of a matrix and a scalar.
     *
     * @param m Left hand matrix.
     * @param scalar Right hand scalar.
     * @return A truth matrix.
     */
    friend Matrix operator!=(Matrix m, double scalar) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return x != scalar ? 1.0 : 0.0; });
        return m;
    }

    /**
     * @brief Performs an elementwise not equal to comparison of a scalar and a matrix.
     *
     * @param scalar Left hand scalar.
     * @param m Right hand matrix.
     * @return A truth matrix.
     */
    friend Matrix operator!=(double scalar, Matrix m) {
        std::ranges::transform(m, m.begin(),
                               [scalar](double x) { return scalar != x ? 1.0 : 0.0; });
        return m;
    }

    // Stream

    /**
     * @brief Writes a readable representation of a matrix to an output stream.
     *
     * Matrices are formatted with one row per line, for example:
     * [[1, 2, 3]
     *  [4, 5, 6]]
     *
     * An empty matrix is written as [].
     *
     * @param out The output stream to write to.
     * @param m The matrix to be written.
     * @return A reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
        if (m.data.empty()) {
            return out << "[]";
        }

        std::streamsize max_width = 0;
        for (double val : m.data) {
            std::ostringstream ss;
            ss.copyfmt(out);  // Respect current stream flags (e.g. std::fixed)
            ss << val;
            max_width = std::max(max_width, static_cast<std::streamsize>(ss.str().length()));
        }

        for (int i = 0; i < m.nrows; ++i) {
            out << (i == 0 ? "[[" : " [");
            for (int j = 0; j < m.ncols; ++j) {
                // std::setw applies padding right before printing the token
                out << std::setw(max_width) << m.data[static_cast<std::size_t>(i) * m.ncols + j];
                if (j < m.ncols - 1) {
                    out << ", ";
                }
            }
            out << (i == m.nrows - 1 ? "]]" : "]\n");
        }
        return out;
    }

   private:
    int nrows;
    int ncols;
    std::vector<double> data;

    /**
     * @brief Converts a matrix index (i, j) into the corresponding index of data.
     *
     * Matrices are row-major so the formula i * ncols + j is used.
     *
     * @param i Row index.
     * @param j Column index.
     * @throw std::out_of_range if i or j is negative, if i >= nrows, or j >= ncols.
     * @return The index of data corresponding to (i, j).
     */
    [[nodiscard]] int flatIndex(int i, int j) const {
        if (i < 0 || i >= nrows || j < 0 || j >= ncols) {
            std::stringstream s;
            s << "matrix: require 0 <= i < " << nrows << " and 0 <= j < " << ncols;
            throw std::out_of_range(s.str());
        }
        return i * ncols + j;
    }

    /**
     * @brief Checks that constructor dimensions are positive or throws an error.
     *
     * @param dim A matrix row or column dimension.
     * @param name The name of the input dimension.
     * @throw std::invalid_argument if dim is not positive.
     * @return The dimension.
     */
    static int validDimension(int dim, const std::string_view name) {
        if (dim <= 0) {
            std::stringstream s;
            s << "Matrix: " << name << " must be positive";
            throw std::invalid_argument(s.str());
        }
        return dim;
    }

};  // class Matrix

/**
 * @brief Multiplies one matrix by another using matrix multiplication.
 * @throw std::invalid_argument if the number of columns of the left hand side does not equal the
 * number of rows of the right hand side.
 */
[[nodiscard]] Matrix operator*(const Matrix& lhs, const Matrix& rhs);

}  // namespace FinMaths::Maths

#endif
