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
    Matrix rowMatrix{1, 3, vec};
    Matrix colMatrix{3, 1, vec};
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

TEST_F(MatrixTest, RowColVectorInit) {
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

TEST_F(MatrixTest, GetRowCol) {
    Matrix row = nested.row(1);
    Matrix col = nested.col(1);

    EXPECT_TRUE(row.equals(Matrix(1, 3, {4.0, 5.0, 6.0})));
    EXPECT_TRUE(col.equals(Matrix(2, 1, {2.0, 5.0})));
}

TEST_F(MatrixTest, GetRowColBounds) {
    EXPECT_THROW((void)nested.row(-1), std::out_of_range);
    EXPECT_THROW((void)nested.row(2), std::out_of_range);
    EXPECT_THROW((void)nested.col(-1), std::out_of_range);
    EXPECT_THROW((void)nested.col(3), std::out_of_range);
}

TEST_F(MatrixTest, SetRowCol) {
    Matrix row(1, 3, 1.0);
    nested.setRow(1, row, 0);
    EXPECT_TRUE(nested.equals(Matrix({{1.0, 2.0, 3.0}, {1.0, 1.0, 1.0}})));

    Matrix col(2, 1, 4.0);
    nested.setCol(2, col, 0);
    EXPECT_TRUE(nested.equals(Matrix({{1.0, 2.0, 4.0}, {1.0, 1.0, 4.0}})));
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
    EXPECT_TRUE(nested.equals(Matrix({{1.0, 2.0, 3.0}, {4.0, 5.0, 7.0}})));
    nested(0, 0) = -1.0;
    EXPECT_TRUE(nested.equals(Matrix({{-1.0, 2.0, 3.0}, {4.0, 5.0, 7.0}})));

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

TEST_F(MatrixTest, Converters) {
    EXPECT_EQ(rowMatrix.asVector(), vec);
    EXPECT_EQ(colMatrix.asVector(), vec);
    EXPECT_DOUBLE_EQ(scalar.asScalar(), 23.0);

    auto rowSpan = rowMatrix.asSpan();
    auto colSpan = colMatrix.asSpan();
    EXPECT_EQ(rowSpan.size(), vec.size());
    EXPECT_EQ(colSpan.size(), vec.size());
    EXPECT_TRUE(std::ranges::equal(rowSpan.begin(), rowSpan.end(), vec.begin(), vec.end()));
    EXPECT_TRUE(std::ranges::equal(colSpan.begin(), colSpan.end(), vec.begin(), vec.end()));

    const auto& ref = rowMatrix.asVector();
    EXPECT_EQ(ref.data(), &rowMatrix.at(0, 0));

    EXPECT_THROW((void)nested.asVector(), std::invalid_argument);
    EXPECT_THROW((void)nested.asSpan(), std::invalid_argument);
    EXPECT_THROW((void)nested.asScalar(), std::invalid_argument);
    EXPECT_THROW((void)rowMatrix.asScalar(), std::invalid_argument);
    EXPECT_THROW((void)colMatrix.asScalar(), std::invalid_argument);
}

TEST(Matrix, Arithmetic) {
    Matrix z{3, 2};
    Matrix m{3, 2, 1.0};
    Matrix n{3, 2, 1.0};

    EXPECT_TRUE(m.isApprox(n));
    EXPECT_TRUE(m.isApprox(-(-m)));
    EXPECT_TRUE((1 + m).isApprox(2 * n));
    EXPECT_TRUE((1 + m).isApprox(n * 2));
    EXPECT_TRUE((m + 1).isApprox(n * 2));
    EXPECT_TRUE((m + m + n).isApprox(3 * n));
    EXPECT_TRUE((m - 1).isApprox(z));
    EXPECT_TRUE((1 - m).isApprox(z));
    EXPECT_TRUE((m - m).isApprox(z));
}

TEST(Matrix, Assignment) {
    const Matrix u{3, 2, 1.0};
    Matrix m{3, 2, 1.0};

    m += 1.0;
    EXPECT_TRUE(m.isApprox(u + u));

    m += 2 * u;
    EXPECT_TRUE(m.isApprox(4 * u));

    m -= 2 * u;
    EXPECT_TRUE(m.isApprox(2 * u));

    m -= 1.0;
    EXPECT_TRUE(m.isApprox(u));

    m *= 8;
    EXPECT_TRUE(m.isApprox(8 * u));
}

TEST(Matrix, Multiplication) {
    Matrix x{{{1, 2, 3}, {4, 5, 6}}};
    Matrix y{{{10, 11}, {20, 21}, {30, 31}}};
    Matrix z{{{140, 146}, {320, 335}}};

    EXPECT_TRUE(z.isApprox(x * y));
    EXPECT_THROW(void(x * x), std::invalid_argument);
}

TEST(Matrix, Elementwise) {
    const Matrix unity{3, 2, 1.0};
    const Matrix m{3, 2, 3.0};

    EXPECT_TRUE(m.exp().isApprox(std::exp(3.0) * unity));
    EXPECT_TRUE(m.log().isApprox(std::log(3.0) * unity));
    EXPECT_TRUE(m.sqrt().isApprox(std::sqrt(3.0) * unity));
    EXPECT_TRUE(m.pow(0.4).isApprox(std::pow(3.0, 0.4) * unity));
    EXPECT_TRUE(m.positivePart().isApprox(m));
    EXPECT_TRUE(m.negativePart().isApprox(0.0 * unity));
    EXPECT_TRUE((-m).negativePart().isApprox(-m));
    EXPECT_TRUE((-m).positivePart().isApprox(0.0 * unity));
}

TEST(Matrix, Comparison) {
    const Matrix falseM{3, 2};
    const Matrix trueM{3, 2, 1.0};
    const Matrix three{3, 2, 3.0};
    const Matrix four{3, 2, 4.0};

    EXPECT_TRUE(trueM.equals(three < four));
    EXPECT_TRUE(falseM.equals(four < three));
    EXPECT_TRUE(falseM.equals(three < three));

    EXPECT_TRUE(trueM.equals(three <= four));
    EXPECT_TRUE(falseM.equals(four <= three));
    EXPECT_TRUE(trueM.equals(three <= three));

    EXPECT_TRUE(falseM.equals(three > four));
    EXPECT_TRUE(trueM.equals(four > three));
    EXPECT_TRUE(falseM.equals(three > three));

    EXPECT_TRUE(falseM.equals(three >= four));
    EXPECT_TRUE(trueM.equals(four >= three));
    EXPECT_TRUE(trueM.equals(three >= three));

    EXPECT_TRUE(falseM.equals(three == four));
    EXPECT_TRUE(falseM.equals(four == three));
    EXPECT_TRUE(trueM.equals(three == three));

    EXPECT_TRUE(trueM.equals(three != four));
    EXPECT_TRUE(trueM.equals(four != three));
    EXPECT_TRUE(falseM.equals(three != three));

    EXPECT_TRUE(trueM.equals(3.0 < four));
    EXPECT_TRUE(falseM.equals(4.0 < three));
    EXPECT_TRUE(falseM.equals(3.0 < three));

    EXPECT_TRUE(trueM.equals(3.0 <= four));
    EXPECT_TRUE(falseM.equals(4.0 <= three));
    EXPECT_TRUE(trueM.equals(3.0 <= three));

    EXPECT_TRUE(falseM.equals(3.0 > four));
    EXPECT_TRUE(trueM.equals(4.0 > three));
    EXPECT_TRUE(falseM.equals(3.0 > three));

    EXPECT_TRUE(falseM.equals(3.0 >= four));
    EXPECT_TRUE(trueM.equals(4.0 >= three));
    EXPECT_TRUE(trueM.equals(3.0 >= three));

    EXPECT_TRUE(falseM.equals(3.0 == four));
    EXPECT_TRUE(falseM.equals(4.0 == three));
    EXPECT_TRUE(trueM.equals(3.0 == three));

    EXPECT_TRUE(trueM.equals(3.0 != four));
    EXPECT_TRUE(trueM.equals(4.0 != three));
    EXPECT_TRUE(falseM.equals(3.0 != three));

    EXPECT_TRUE(trueM.equals(three < 4.0));
    EXPECT_TRUE(falseM.equals(four < 3.0));
    EXPECT_TRUE(falseM.equals(three < 3.0));

    EXPECT_TRUE(trueM.equals(three <= 4.0));
    EXPECT_TRUE(falseM.equals(four <= 3.0));
    EXPECT_TRUE(trueM.equals(three <= 3.0));

    EXPECT_TRUE(falseM.equals(three > 4.0));
    EXPECT_TRUE(trueM.equals(four > 3.0));
    EXPECT_TRUE(falseM.equals(three > 3.0));

    EXPECT_TRUE(falseM.equals(three >= 4.0));
    EXPECT_TRUE(trueM.equals(four >= 3.0));
    EXPECT_TRUE(trueM.equals(three >= 3.0));

    EXPECT_TRUE(falseM.equals(three == 4.0));
    EXPECT_TRUE(falseM.equals(four == 3.0));
    EXPECT_TRUE(trueM.equals(three == 3.0));

    EXPECT_TRUE(trueM.equals(three != 4.0));
    EXPECT_TRUE(trueM.equals(four != 3.0));
    EXPECT_TRUE(falseM.equals(three != 3.0));
}

TEST(Matrix, Trace) {
    Matrix x{1, 1, 1.0};
    Matrix y{{{1.0, 2.0}, {3.0, 4.0}}};
    Matrix z{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}};

    EXPECT_DOUBLE_EQ(x.trace(), 1.0);
    EXPECT_DOUBLE_EQ(y.trace(), 5.0);
    EXPECT_DOUBLE_EQ(z.trace(), 15.0);
}

TEST_F(MatrixTest, Transpose) {
    EXPECT_TRUE((m32.transpose()).equals(Matrix{2, 3}));
    EXPECT_TRUE(m23.transpose().equals(Matrix{3, 2, 1.0}));
    EXPECT_TRUE(scalar.transpose().equals(scalar));
    EXPECT_TRUE(nested.transpose().transpose().equals(nested));
}

TEST_F(MatrixTest, Symmetric) {
    EXPECT_FALSE(m32.isSquare());
    EXPECT_FALSE(m23.isSymmetric());

    EXPECT_TRUE((nested * nested.transpose()).isSymmetric());
    EXPECT_TRUE((nested.transpose() * nested).isSymmetric());
}

TEST(Matrix, Identity) {
    EXPECT_TRUE(Matrix::identity(3).equals(Matrix({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}})));
    EXPECT_THROW((void)Matrix::identity(0), std::invalid_argument);
}

TEST(Matrix, StreamOutput) {
    std::ostringstream ss;
    ss << Matrix(2, 2, {1.0, 2.0, 3.0, 4.0});
    EXPECT_EQ(ss.str(), "[[1, 2]\n [3, 4]]");
}
