#include "libs/core/tensor.h"

#include <gtest/gtest.h>

namespace {

// ── Tensor::num_elements ─────────────────────────────────────────────────────
TEST(TensorTest, NumElements3D) {
  nvidia::core::Tensor t;
  t.shape = {3, 4, 5};
  EXPECT_EQ(t.num_elements(), 60);
}

TEST(TensorTest, NumElementsEmpty) {
  nvidia::core::Tensor t;
  EXPECT_EQ(t.num_elements(), 0);
}

// ── matmul ───────────────────────────────────────────────────────────────────
TEST(MatmulTest, IdentityLeft) {
  // I @ B == B
  nvidia::core::Tensor I{{1, 0, 0, 1}, {2, 2}};
  nvidia::core::Tensor B{{5, 6, 7, 8}, {2, 2}};
  auto C = nvidia::core::matmul(I, B);
  EXPECT_FLOAT_EQ(C.data[0], 5.0f);
  EXPECT_FLOAT_EQ(C.data[1], 6.0f);
  EXPECT_FLOAT_EQ(C.data[2], 7.0f);
  EXPECT_FLOAT_EQ(C.data[3], 8.0f);
}

TEST(MatmulTest, Basic2x2) {
  // [[1,2],[3,4]] @ [[5,6],[7,8]] = [[19,22],[43,50]]
  nvidia::core::Tensor A{{1, 2, 3, 4}, {2, 2}};
  nvidia::core::Tensor B{{5, 6, 7, 8}, {2, 2}};
  auto C = nvidia::core::matmul(A, B);
  EXPECT_FLOAT_EQ(C.data[0], 19.0f);
  EXPECT_FLOAT_EQ(C.data[1], 22.0f);
  EXPECT_FLOAT_EQ(C.data[2], 43.0f);
  EXPECT_FLOAT_EQ(C.data[3], 50.0f);
}

TEST(MatmulTest, ShapeMismatchThrows) {
  nvidia::core::Tensor A{{1, 2, 3}, {1, 3}};
  nvidia::core::Tensor B{{4, 5}, {1, 2}};  // inner: 3 vs 1 → mismatch
  EXPECT_THROW(nvidia::core::matmul(A, B), std::invalid_argument);
}

// ── relu ─────────────────────────────────────────────────────────────────────
TEST(ReluTest, ClampsNegative) {
  nvidia::core::Tensor x{{-2.0f, -0.0f, 0.5f, 3.0f}, {2, 2}};
  auto y = nvidia::core::relu(x);
  EXPECT_FLOAT_EQ(y.data[0], 0.0f);
  EXPECT_FLOAT_EQ(y.data[1], 0.0f);
  EXPECT_FLOAT_EQ(y.data[2], 0.5f);
  EXPECT_FLOAT_EQ(y.data[3], 3.0f);
}

// ── add ──────────────────────────────────────────────────────────────────────
TEST(AddTest, Elementwise) {
  nvidia::core::Tensor a{{1, 2}, {1, 2}};
  nvidia::core::Tensor b{{3, 4}, {1, 2}};
  auto c = nvidia::core::add(a, b);
  EXPECT_FLOAT_EQ(c.data[0], 4.0f);
  EXPECT_FLOAT_EQ(c.data[1], 6.0f);
}

TEST(AddTest, ShapeMismatchThrows) {
  nvidia::core::Tensor a{{1, 2}, {1, 2}};
  nvidia::core::Tensor b{{3, 4, 5}, {1, 3}};
  EXPECT_THROW(nvidia::core::add(a, b), std::invalid_argument);
}

}  // namespace
