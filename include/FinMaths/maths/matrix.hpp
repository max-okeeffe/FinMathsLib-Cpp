#ifndef FINMATHS_MATRIX_HPP
#define FINMATHS_MATRIX_HPP

#include <cstdint>
#include <span>
#include <sstream>
#include <stdexcept>
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
    Matrix(int nrows, int ncols, const std::vector<double>& values);

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
    explicit Matrix(const std::vector<double>& values, VectorType type = VectorType::Row);

    /**
     * @brief Construct a matrix from a vector of vectors.
     *
     * @param values a vector of vector of doubles.
     *
     * @throw std::invalid_argument if values is empty or not all elements of values have the same
     * positive size.
     */
    explicit Matrix(const std::vector<std::vector<double> >& values);

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
    std::vector<double>::iterator begin() noexcept {
        return data.begin();
    }
    /**
     * @brief Return an iterator to the end of the matrix data.
     */
    std::vector<double>::iterator end() noexcept {
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
        return m += scalar;
    }

    /**
     * @brief Adds a scalar and a matrix.
     */
    friend Matrix operator+(double scalar, Matrix m) {
        return m += scalar;
    }

    /**
     * @brief Adds two matrices together.
     * @throw std::invalid_argument if the matrices do not have the same dimensions.
     */
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) {
        return lhs += rhs;
    }

    /**
     * @brief Returns the additive inverse of a matrix.
     */
    friend Matrix operator-(Matrix m) {
        return m *= -1.0;
    }

    /**
     * @brief Subtracts a scalar from a matrix.
     */
    friend Matrix operator-(Matrix m, double scalar) {
        return m -= scalar;
    }

    /**
     * @brief Subtracts a matrix from a scalar.
     */
    friend Matrix operator-(double scalar, Matrix m) {
        return -std::move(m) += scalar;
    }

    /**
     * @brief Subtracts one matrix from another.
     * @throw std::invalid_argument if the matrices have different dimensions.
     */
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) {
        return lhs -= rhs;
    }

    /**
     * @brief Multiplies a matrix by a scalar.
     */
    friend Matrix operator*(Matrix m, double scalar) {
        return m *= scalar;
    }

    /**
     * @brief Multiplies a scalar by a matrix.
     */
    friend Matrix operator*(double scalar, Matrix m) {
        return m *= scalar;
    }

    /**
     * @brief Divides a matrix by a scalar.
     * @throw std::logic_error if scalar is zero.
     */
    friend Matrix operator/(Matrix m, double scalar) {
        return m /= scalar;
    }

    // Comparison operators

    /**
     * @brief Tests whether two matrices are equal.
     *
     * Implemented by ensuring dimensions and elements are equal.
     */
    [[nodiscard]] bool operator==(const Matrix& other) const noexcept;

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
     */
    [[nodiscard]] bool isApprox(const Matrix& other, double absTolerance = 1e-12,
                                double relTolerance = 1e-12) const noexcept;

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
