#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "FinMaths/maths/matrix.hpp"

using namespace FinMaths::Maths;

class MatrixTest : public ::testing::Test {
   protected:
    Matrix m32{3, 2};
    Matrix m23{2, 3, 1.0};
    Matrix scalar{23.0};
    Matrix fromVector{2, 3, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};
    std::vector<double> vec{1.0, 2.0, 3.0};
    Matrix nested{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}};
};

TEST_F(MatrixTest, DefaultValue) {
    EXPECT_EQ(m32.nRows(), 3);
    EXPECT_EQ(m32.nCols(), 2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            EXPECT_DOUBLE_EQ(m32.get(i, j), 0.0);
        }
    }
}

TEST_F(MatrixTest, SetValue) {
    EXPECT_EQ(m23.nRows(), 2);
    EXPECT_EQ(m23.nCols(), 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(m23.get(i, j), 1.0);
        }
    }
}

TEST_F(MatrixTest, Singleton) {
    EXPECT_EQ(scalar.nRows(), 1);
    EXPECT_EQ(scalar.nCols(), 1);
    EXPECT_DOUBLE_EQ(scalar.get(0, 0), 23.0);
}

TEST(Constructor, Dimensions) {
    EXPECT_THROW(Matrix m(0, 1), std::invalid_argument);
    EXPECT_THROW(Matrix m(1, -1), std::invalid_argument);
}

TEST_F(MatrixTest, FromVector) {
    EXPECT_EQ(fromVector.nRows(), 2);
    EXPECT_EQ(fromVector.nCols(), 3);
    int count = 1;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(fromVector.get(i, j), static_cast<double>(count));
            ++count;
        }
    }
}

TEST_F(MatrixTest, MismatchedDimensions) {
    std::vector<double> v;
    EXPECT_THROW(Matrix m(1, 1, v), std::invalid_argument);
    EXPECT_THROW(Matrix m(1, 2, vec), std::invalid_argument);
    EXPECT_THROW(Matrix m(-1, 2, vec), std::invalid_argument);
    EXPECT_THROW(Matrix m(1, 0, vec), std::invalid_argument);
}

TEST_F(MatrixTest, RowColVector) {
    Matrix rowMatrix(vec, VectorType::Row);
    Matrix colMatrix(vec, VectorType::Column);
    Matrix m(vec);
    EXPECT_EQ(rowMatrix.nRows(), 1);
    EXPECT_EQ(rowMatrix.nCols(), 3);
    EXPECT_EQ(colMatrix.nRows(), 3);
    EXPECT_EQ(colMatrix.nCols(), 1);
    EXPECT_EQ(m.nRows(), 1);
    EXPECT_EQ(m.nCols(), 3);
}

TEST(RowColVector, Empty) {
    std::vector<double> v;
    EXPECT_THROW(Matrix m(v, VectorType::Row), std::invalid_argument);
    EXPECT_THROW(Matrix m(v, VectorType::Column), std::invalid_argument);
    EXPECT_THROW(Matrix m(v), std::invalid_argument);
}

TEST_F(MatrixTest, Nested) {
    EXPECT_EQ(nested.nRows(), 2);
    EXPECT_EQ(nested.nCols(), 3);
    int count = 1;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ(nested.get(i, j), static_cast<double>(count));
            ++count;
        }
    }
}

TEST(Nested, MismatchedDimensions) {
    std::vector<std::vector<double>> v;
    EXPECT_THROW(Matrix m(v), std::invalid_argument);

    std::vector<double> w;
    v.push_back(w);
    EXPECT_THROW(Matrix m(v), std::invalid_argument);

    v = {{1.0, 2.0, 3.0}};
    v.push_back(w);
    EXPECT_THROW(Matrix m(v), std::invalid_argument);

    v = {{1.0, 2.0}, {1.0, 2.0}, {1.0, 2.0, 3.0}};
    EXPECT_THROW(Matrix m(v), std::invalid_argument);
}

TEST_F(MatrixTest, Size) {
    EXPECT_EQ(m23.size(), 6);
    EXPECT_EQ(m32.size(), 6);
    EXPECT_EQ(scalar.size(), 1);
    EXPECT_EQ(fromVector.size(), 6);
    EXPECT_EQ(nested.size(), 6);
}

TEST_F(MatrixTest, RowCol) {
    Matrix row = nested.row(1);
    Matrix col = nested.col(1);

    EXPECT_TRUE(row == Matrix(1, 3, {4.0, 5.0, 6.0}));
    EXPECT_TRUE(col == Matrix(2, 1, {2.0, 5.0}));
}

TEST_F(MatrixTest, RowColBounds) {
    EXPECT_THROW(nested.row(-1), std::out_of_range);
    EXPECT_THROW(nested.row(2), std::out_of_range);
    EXPECT_THROW(nested.col(-1), std::out_of_range);
    EXPECT_THROW(nested.col(3), std::out_of_range);
}

TEST_F(MatrixTest, SetRowCol) {
    Matrix row(1, 3, 1.0);
    nested.setRow(1, row, 0);
    EXPECT_TRUE(nested == Matrix({{1.0, 2.0, 3.0}, {1.0, 1.0, 1.0}}));

    Matrix col(2, 1, 4.0);
    nested.setCol(2, col, 0);
    EXPECT_TRUE(nested == Matrix({{1.0, 2.0, 4.0}, {1.0, 1.0, 4.0}}));
}

TEST_F(MatrixTest, SetMismatchedDimensions) {
    Matrix m(1, 4);
    EXPECT_THROW(nested.setRow(0, m, 0), std::invalid_argument);
    EXPECT_THROW(nested.setCol(0, m, 0), std::invalid_argument);
}

TEST_F(MatrixTest, Access) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(nested.at(i, j), nested.get(i, j));
            EXPECT_DOUBLE_EQ(nested(i, j), nested.get(i, j));
        }
    }
    nested.at(1, 2) = 7.0;
    EXPECT_TRUE(nested == Matrix({{1.0, 2.0, 3.0}, {4.0, 5.0, 7.0}}));
    nested(0, 0) = -1.0;
    EXPECT_TRUE(nested == Matrix({{-1.0, 2.0, 3.0}, {4.0, 5.0, 7.0}}));

    EXPECT_THROW(nested.at(-1, 3), std::out_of_range);
}

TEST_F(MatrixTest, IsApprox) {
    Matrix a(2, 2, {1.0, 2.0, 3.0, 4.0});
    Matrix b(2, 2, {1.0 + 1e-11, 2.0, 3.0, 4.0});
    Matrix c(2, 2, {1.0 + 1e-9, 2.0, 3.0, 4.0});
    EXPECT_TRUE(a.isApprox(b, 1e-10));
    EXPECT_FALSE(a.isApprox(c, 1e-10));
    EXPECT_FALSE(a.isApprox(Matrix(3, 2), 1e-10));
}
